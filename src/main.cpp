#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <thread>
#include <chrono>
#include <iostream>
//      |
//      |
//HELP "\/"
#include "/Users/shared/CLionProjects/Genbliothinothon/res/shaders/sine_wave.glsl"
#include "Genbliothinothon.h"



void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	static int sample = 0;
	int32_t* pOutputS32 = static_cast<int32_t*>(pOutput);
//================================================================
	for (ma_uint32 i = 0; i < frameCount; i++) {
		pOutputS32[i] = triangle(sample++, 440, 0.03f);
	}
}

int main() {
	genbliothinothon::Genbliothinothon genbliothinothon;

	ma_device_config config = ma_device_config_init(ma_device_type_playback);
	config.playback.format = ma_format_s32;
	config.playback.channels = 1;
	config.sampleRate = 96000;
	config.dataCallback = data_callback;

	ma_device device;
	if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
		return -1;
	}

	ma_device_start(&device);

	genbliothinothon.run();

	ma_device_uninit(&device);
	return 0;
}
