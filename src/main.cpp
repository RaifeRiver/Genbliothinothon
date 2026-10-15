#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <thread>
#include <chrono>
#include "/Users/Shared/CLionProjects/Genbliothinothon/res/shaders/sine_wave.glsl"
#include "Genbliothinothon.h"

//int main() {
//	genbliothinothon::Genbliothinothon genbliothinothon;
//	genbliothinothon.run();
//}


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	static int sample = 0;
	float* output = static_cast<float*>(pOutput);
//================================================================
	for (ma_uint32 i = 0; i < frameCount; i++) {
		output[i] = triangle(sample++, 440, 0.05);//sine_wave(sample++, 440, 0.05);
	}
}

int main() {
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
	std::this_thread::sleep_for(std::chrono::seconds(3));

	ma_device_uninit(&device);
	return 0;
}
