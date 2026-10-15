#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <thread>
#include <chrono>
#include <iostream>

#include "/Users/Shared/CLionProjects/Genbliothinothon/res/shaders/sine_wave.glsl"
#include "Genbliothinothon.h"



void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	static int sample = 0;
	float* output = static_cast<float*>(pOutput);
//================================================================
	for (ma_uint32 i = 0; i < frameCount; i++) {
		float OAN = sine_wave(sample++, 23.0, 57.0)+300.0;
		output[i] = triangle(sample, OAN, 0.05f);
	}
}

int main() {
	genbliothinothon::Genbliothinothon genbliothinothon;

	ma_device_config config = ma_device_config_init(ma_device_type_playback);
	config.playback.format = ma_format_f32;
	config.playback.channels = 1;
	config.sampleRate = 96000;
	config.dataCallback = data_callback;

	ma_device device;
	if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
		return -1;
	}

	ma_device_start(&device);

	std::cout << genbliothinothon.shouldClose() << std::endl;
	while (genbliothinothon.shouldClose()) {
		genbliothinothon.renderFrame();
	}

	ma_device_uninit(&device);
	return 0;
}
