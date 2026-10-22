#include "AudioPlayer.h"

#include <iostream>

#include "module/Output.h"
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

		std::string generatedCode;
		std::string generatedFunctions;

		std::unordered_map<uint32_t, std::unique_ptr<module::Module>>& modules = moduleManager.getModules();

		std::vector generatedModules(modules.size(), false);
		std::vector generatingModules(modules.size(), false);
		const std::function<void(uint32_t)> generateModule = [&](const uint32_t module) {
			if (generatedModules[module]) {
				return;
			}
			if (generatingModules[module]) {
				throw std::runtime_error("Recursive part generation");
			}
			generatingModules[module] = true;

			const std::vector<std::unique_ptr<module::part::Part>>& parts = modules[module]->getParts();
			std::unordered_map<std::string, std::string> inputStrings;
			for (const std::unique_ptr<module::part::Part>& part: parts) {
				if (const auto sendTarget = dynamic_cast<module::part::SendTarget*>(part.get())) {
					uint32_t channelCount = 0;
					for (auto [parentID, partIndex] : sendTarget->getInputs()) {
						channelCount = std::max(channelCount, reinterpret_cast<module::part::Send*>(modules[parentID]->getParts()[partIndex].get())->getChannelCount());
					}
					const module::part::CombiningMode combiningMode = sendTarget->getCombiningMode();
					std::string combiningOperator;
					switch (combiningMode) {
						case module::part::CombiningMode::AVERAGE:
						case module::part::CombiningMode::SUM:
							combiningOperator = "+";
							break;
							break;
						case module::part::CombiningMode::OR:
							combiningOperator = "|";
							break;
					}
					for (uint32_t channel = 0; channel < channelCount; channel++) {
						std::string channelStr = std::to_string(channel);
						std::string inputString;
						if (combiningMode == module::part::CombiningMode::AVERAGE) {
							inputString = "(";
						}
						uint32_t inputCount = 0;
						for (auto [parentID, partIndex] : sendTarget->getInputs()) {
							module::Module* parent = modules[parentID].get();
							for (uint32_t parentIndex = 0; parentIndex < modules.size(); parentIndex++) {
								if (parent == modules[parentIndex].get()) {
									generateModule(parentIndex);
									inputString += reinterpret_cast<module::part::Send*>(parent->getParts()[partIndex].get())->getName() + channelStr + "_" + std::to_string(parentIndex) += combiningOperator;
									inputCount++;
								}
							}
						}
						if (inputCount != 0) {
							inputString.pop_back();
							if (combiningMode == module::part::CombiningMode::AVERAGE) {
								inputString += ")*" + std::to_string(1.0f / static_cast<float>(inputCount)) + "f";
							}
							inputStrings[sendTarget->getName() + channelStr + "_"] = inputString;
						}
					}
				}
			}
			generatedFunctions += modules[module]->generateFunctions(module);

			generatedCode += modules[module]->generateGLSL(module, inputStrings) + "\n";

			generatedModules[module] = true;
		};
		bool foundOutput = false;
		for (uint32_t module = 0; module < modules.size(); module++) {
			if (dynamic_cast<module::Output*>(modules[module].get())) {
				if (foundOutput) {
					throw std::runtime_error("Multiple outputs found");
				}
				generateModule(module);
				foundOutput = true;
			}
		}

		if (!foundOutput) {
			generatedCode += "float leftAudio=0.0f;\nfloat rightAudio=0.0f;\n";
		}

		std::cout << generatedFunctions << std::endl;
		std::cout << generatedCode << std::endl;

		fragmentShader.replace(fragmentShader.find("// insert functions here"), 24, generatedFunctions);
		fragmentShader.replace(fragmentShader.find("// insert audio code here"), 25, generatedCode);

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
