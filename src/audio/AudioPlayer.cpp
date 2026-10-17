#include "AudioPlayer.h"

#include <iostream>

namespace genbliothinothon::audio {
	AudioPlayer::AudioPlayer() {
		mShader = std::make_unique<render::Shader>("../res/shaders/audio/audio.vert", "../res/shaders/audio/audio.frag");

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
			mOscilloscopeShader = std::make_unique<render::Shader>("../res/shaders/audio/oscilloscope.vert", "../res/shaders/audio/oscilloscope.frag");
		}
	}

	void AudioPlayer::update(render::RenderContext& renderContext) {
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
