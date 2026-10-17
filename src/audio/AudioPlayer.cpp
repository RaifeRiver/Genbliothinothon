#include "AudioPlayer.h"

#include <iostream>

#include "module/Output.h"
#include "module/part/Send.h"

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
		outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;
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

	void AudioPlayer::rebuildShader(const std::vector<std::unique_ptr<module::Module>>& modules) {
		std::string vertexShader = render::loadShaderSource("../res/shaders/audio/audio.vert");
		std::string fragmentShader = render::loadShaderSource("../res/shaders/audio/audio.frag");

		std::string generatedCode;

		std::vector generatedModules(modules.size(), false);
		std::vector generatingModules(modules.size(), false);
		std::vector<uint32_t> outputs;
		const std::function generateModule = [&](const size_t module) {
			if (generatedModules[module]) {
				return;
			}
			if (generatingModules[module]) {
				throw std::runtime_error("Recursive send loop");
			}
			generatingModules[module] = true;
			std::string sendString = "(";
			uint32_t sendCount = 0;
			for (uint32_t i = 0; i < modules.size(); i++) {
				if (module == i) {
					continue;
				}
				for (std::unique_ptr<module::part::Part>& part: modules[i]->getParts()) {
					if (const module::part::Send* send = dynamic_cast<module::part::Send*>(part.get())) {
						if (const module::Module* sendTarget = send->getSendTarget(); sendTarget != modules[module].get()) {
							continue;
						}
						sendString += "send" + std::to_string(i) + "+";
						sendCount++;
					}
				}
			}
			if (sendCount == 0) {
				sendString = "0.0f";
			}
			else {
				sendString.pop_back();
				sendString += ")/" + std::to_string(sendCount) + ".0f";
			}
			generatedCode += "float send" + std::to_string(module) + "=" + modules[module]->generateGLSL(sendString) + ";\n";
			if (dynamic_cast<module::Output*>(modules[module].get())) {
				outputs.push_back(module);
			}
			generatingModules[module] = false;
			generatedModules[module] = true;
		};
		for (uint32_t module = 0; module < modules.size(); module++) {
			generateModule(module);
		}
		if (outputs.empty()) {
			generatedCode += "float audio = 0.0f;";
		}
		else {
			generatedCode += "float audio = ";
			for (const uint32_t output : outputs) {
				generatedCode += "send" + std::to_string(output) + "+";
			}
			generatedCode.pop_back();
			generatedCode += ';';
		}
std::cout << generatedCode << std::endl;
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
