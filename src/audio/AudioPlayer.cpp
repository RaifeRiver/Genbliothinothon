#include "AudioPlayer.h"

#include <iostream>
#include <ranges>

#include "module/Oscillator.h"
#include "module/Output.h"
#include "module/part/Background.h"
#include "module/part/Send.h"
#include "module/part/SendTarget.h"

namespace genbliothinothon::audio {
	AudioPlayer::AudioPlayer() {
		float bufferData[] = {
			-1.0f,  1.0f,
			-1.0f, -1.0f,
			 1.0f,  1.0f,
			 1.0f, -1.0f
		};
		mVertexBuffer = std::make_unique<render::Buffer>(bufferData, sizeof(bufferData));

		mOutputImage = std::make_unique<render::Image>(glm::uvec2{128, 128}, GL_RG32F);

		mVAO.setAttribute(0, *mVertexBuffer, 2, GL_FLOAT);

		if (Pa_Initialize() != paNoError) {
			throw std::runtime_error("Failed to initialise audio output");
		}

		PaStreamParameters outputParameters;
		outputParameters.device = Pa_GetDefaultOutputDevice();
		outputParameters.channelCount = 2;
		outputParameters.sampleFormat = paFloat32;
		outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = nullptr;
		if (Pa_OpenStream(&mStream, nullptr, &outputParameters, 96000, 16384, paClipOff, nullptr, nullptr) != paNoError) {
			throw std::runtime_error("Failed to open audio output stream");
		}
		if (Pa_StartStream(mStream) != paNoError) {
			throw std::runtime_error("Failed to start audio output stream");
		}

		if (ENABLE_SCOPE) {
			mOscilloscopeShader = render::Shader::loadFromFiles("../res/shaders/audio/oscilloscope.vert", "../res/shaders/audio/oscilloscope.frag");
		}
	}

	void AudioPlayer::rebuildShader(module::ModuleManager& moduleManager) {
		std::string vertexShader = render::loadShaderSource("../res/shaders/audio/audio.vert");
		std::string fragmentShader = render::loadShaderSource("../res/shaders/audio/audio.frag");

		std::string generatedFunctions;

		std::unordered_map<uint32_t, std::unique_ptr<module::Module>>& modules = moduleManager.getModules();

		std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::vector<std::pair<std::string, std::pair<uint32_t, std::string>>>>> generatedModules;
		std::unordered_map<uint32_t, std::vector<uint32_t>> generatingModules;
		std::function<void(uint32_t)> generateModule;
		auto replaceAll = [](std::string& s, const std::string& replace, const std::string& replacement) {
			size_t pos = 0;
			const size_t replaceLength = replace.length();
			while ((pos = s.find(replace, pos)) != std::string::npos) {
				s.replace(pos, replaceLength, replacement);
				pos += replaceLength;
			}
		};
		const std::function<std::pair<uint32_t, std::string>(uint32_t, module::Module*, module::part::Send*, uint32_t)> generateInput = [&](const uint32_t moduleID, module::Module* module, const module::part::Send* send, const uint32_t channel) {
			generateModule(moduleID);

			if (dynamic_cast<module::Oscillator*>(module)) {
				std::cout << "Generating input for oscillator" << std::endl;
			}
			else {
				std::cout << "Generating input for non-oscillator" << std::endl;
			}

			std::string glsl = module->generateGLSL(send->getName());
			replaceAll(glsl, "$Sample", "sample0");
			replaceAll(glsl, "$Channel", std::to_string(channel));
			replaceAll(glsl, "$Index", std::to_string(moduleID));
			for (const auto& [name, input] : generatedModules[moduleID][channel]) {
				replaceAll(glsl, "$" + name, input.second);
				std::cout << "Replacing $" << name << " with " << input.second << " for channel " << channel << std::endl;
			}
			return std::pair{send->getChannelCount(), glsl};
		};
		auto generateModuleChannel = [&](const uint32_t module, const uint32_t channel) {
			if (generatedModules.contains(module) && generatedModules[module].contains(channel)) {
				return;
			}
			for (const uint32_t c: generatingModules[module]) {
				if (c == channel) {
					throw std::runtime_error("Recursive part generation");
				}
			}
			generatingModules[module].push_back(channel);

			const std::vector<std::unique_ptr<module::part::Part>>& parts = modules[module]->getParts();
			std::vector<std::pair<std::string, std::pair<uint32_t, std::string>>> inputValues;
			std::string parameters = "uint sample0,";
			std::unordered_map<std::string, std::pair<uint32_t, std::string>> outputs;
			for (const std::unique_ptr<module::part::Part>& part: parts) {
				if (const auto sendTarget = dynamic_cast<module::part::SendTarget*>(part.get()); sendTarget) {
					if (sendTarget->getInput().has_value()) {
						auto [inputModuleID, sendID] = sendTarget->getInput().value();
						module::Module* inputModule = modules[inputModuleID].get();
						auto* send = reinterpret_cast<module::part::Send*>(inputModule->getParts()[sendID].get());
						inputValues.emplace_back(sendTarget->getName(), generateInput(inputModuleID, inputModule, send, channel));
					}
					else {
						inputValues.emplace_back(sendTarget->getName(), std::pair{1, sendTarget->getDefaultValueString()});
					}
				}
				else if (const auto send = dynamic_cast<module::part::Send*>(part.get())) {
					for (uint32_t c = 0; c < send->getChannelCount(); c++) {
						parameters += "out " + send->getDataType() + " " + send->getName() + std::to_string(c) + "_,";
						outputs[send->getName() + std::to_string(c) + "_"] = {static_cast<uint32_t>(outputs.size()), send->getDataType()};
					}
				}
			}
			generatedModules[module][channel] = inputValues;
			generatingModules[module].pop_back();
		};
		generateModule = [&](const uint32_t module) {
			if (generatedModules.contains(module)) {
				return;
			}
			if (generatingModules.contains(module)) {
				throw std::runtime_error("Recursive part generation");
			}
			generatingModules[module] = {};

			uint32_t channelCount = 1;
			for (const std::unique_ptr<module::part::Part>& part: modules[module]->getParts()) {
				if (const module::part::Send* send = dynamic_cast<module::part::Send*>(part.get())) {
					channelCount = std::max(channelCount, send->getChannelCount());
				}
			}
			for (uint32_t c = 0; c < channelCount; c++) {
				generateModuleChannel(module, c);
			}

			generatedFunctions += modules[module]->generateFunctions(module);
			generatingModules.erase(module);
		};
		bool foundOutput = false;
		std::string leftString = "float leftAudio=";
		std::string rightString = "float rightAudio=";
		for (auto& module: modules | std::views::values) {
			if (dynamic_cast<module::Output*>(module.get())) {
				if (foundOutput) {
					throw std::runtime_error("Multiple outputs found");
				}
				std::optional<module::part::PartID> leftSend = reinterpret_cast<module::part::SendTarget*>(module->getParts()[1].get())->getInput();
				std::optional<module::part::PartID> rightSend = reinterpret_cast<module::part::SendTarget*>(module->getParts()[2].get())->getInput();
				uint32_t channelCount = std::max(1u, leftSend.has_value() ? reinterpret_cast<module::part::Send*>(modules[leftSend.value().module]->getParts()[leftSend.value().part].get())->getChannelCount() : 0);
				channelCount = std::max(channelCount, rightSend.has_value() ? reinterpret_cast<module::part::Send*>(modules[rightSend.value().module]->getParts()[rightSend.value().part].get())->getChannelCount() : 0);
				for (uint32_t c = 0; c < channelCount; c++) {
					std::string channelStr = std::to_string(c);
					generatedFunctions += "float leftAudio" + channelStr + "(uint sample0){return " + (leftSend.has_value() ? generateInput(leftSend.value().module, modules[leftSend.value().module].get(), reinterpret_cast<module::part::Send*>(modules[leftSend.value().module]->getParts()[leftSend.value().part].get()), c).second : "0.0f") + ";}\n";
					generatedFunctions += "float rightAudio" + channelStr + "(uint sample0){return " + (rightSend.has_value() ? generateInput(rightSend.value().module, modules[rightSend.value().module].get(), reinterpret_cast<module::part::Send*>(modules[rightSend.value().module]->getParts()[rightSend.value().part].get()), c).second : "0.0f") + ";}\n";
					leftString += "leftAudio" + channelStr + "(sample0)+";
					rightString += "rightAudio" + channelStr + "(sample0)+";
				}
				if (!leftSend.has_value()) {
					leftString += "0.0f;";
				}
				if (!rightSend.has_value()) {
					rightString += "0.0f;";
				}
				foundOutput = true;
			}
		}

		if (!foundOutput) {
			leftString = "float leftAudio=0.0f;";
			rightString = "float rightAudio=0.0f;";
		}
		else {
			leftString.back() = ';';
			rightString.back() = ';';
		}

		std::cout << generatedFunctions << std::endl;
		std::cout << leftString << std::endl;
		std::cout << rightString << std::endl;

		fragmentShader.replace(fragmentShader.find("// insert functions here"), 24, generatedFunctions);
		fragmentShader.replace(fragmentShader.find("// insert audio code here"), 25, leftString + "\n" + rightString);

		std::cout << fragmentShader << std::endl;

		mShader = std::make_unique<render::Shader>(vertexShader, fragmentShader);
	}

	void AudioPlayer::update(render::RenderContext& renderContext) {
		if (!mShader) {
			return;
		}

		mVAO.bind();

		if (ENABLE_SCOPE) {
			mOscilloscopeShader->bind();
			mOscilloscopeShader->setUniform("uScreenSize", renderContext.getWindowSize());
			renderContext.drawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		if (Pa_GetStreamWriteAvailable(mStream) < 16384) {
			return;
		}

		if (!mFirst) {
			if (const GLenum status = glClientWaitSync(mFence, 0, 0); status != GL_ALREADY_SIGNALED && status != GL_CONDITION_SATISFIED) {
				return;
			}
			glDeleteSync(mFence);
			float data[32768];
			glBindTexture(GL_TEXTURE_2D, mOutputImage->getTexture());
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_FLOAT, data);
			Pa_WriteStream(mStream, data, 16384);
			if (ENABLE_SCOPE) {
				static bool oscilloscopeSet = false;
				if (!oscilloscopeSet) {
					std::vector<float> oscilloscopeData;
					oscilloscopeData.reserve(512);
					for (int i = 0; i < 1024; i += 2) {
						oscilloscopeData.push_back(data[i]);
					}
					mOscilloscopeShader->setUniform("uFrequencies", oscilloscopeData);
					oscilloscopeSet = true;
				}
			}
		}

		mShader->bind();

		mShader->setUniform("uSampleBase", mNextSampleBase);
		mNextSampleBase += 16384;

		renderContext.setFBO(*mOutputImage);
		renderContext.drawArrays(GL_TRIANGLE_STRIP, 0, 4);
		renderContext.clearFBO();

		mFence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		mFirst = false;
	}

	AudioPlayer::~AudioPlayer() {
		Pa_Terminate();
	}
}
