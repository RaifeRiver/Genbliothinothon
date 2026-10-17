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
	}

	void AudioPlayer::update(render::RenderContext& renderContext) {
		if (Pa_GetStreamWriteAvailable(mStream) < 16384) {
			return;
		}

		if (!mFirst) {
			if (const GLenum status = glClientWaitSync(mFence, 0, 0); status != GL_ALREADY_SIGNALED && status != GL_CONDITION_SATISFIED) {
				return;
			}
			glDeleteSync(mFence);
			unsigned char data[131072];
			glBindTexture(GL_TEXTURE_2D, mOutputImage->getTexture());
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_FLOAT, data);
			Pa_WriteStream(mStream, data, 16384);
		}

		mVAO.bind();
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
