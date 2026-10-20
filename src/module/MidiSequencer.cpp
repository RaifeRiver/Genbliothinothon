#include "MidiSequencer.h"

#include <cmath>
#include <fstream>

#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	MidiSequencer::MidiSequencer(const glm::vec2& pos) : Module(pos, glm::vec2{200.0f, 93.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 93.0f}, "MIDI Sequencer"));
		addPart(std::make_unique<part::RemoveButton>(this));
		mTextInput = &addPart(std::make_unique<part::TextInput>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 47.0f}, glm::vec2{194.0f, 20.0f}, "Frequency", 16));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 70.0f}, glm::vec2{194.0f, 20.0f}, "Volume", 16));
	}

	std::string MidiSequencer::generateFunctions(const uint32_t index) {
		std::ifstream file(mTextInput->getText(), std::ios::binary | std::ios::ate);
		if (!file.is_open()) {
			mLength = 1.0f;
			std::string indexStr = std::to_string(index);
			return "vec2 midiSequencerGetNote0_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote1_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote2_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote3_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote4_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote5_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote6_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote7_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote8_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote9_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote10_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote11_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote12_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote13_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote14_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote15_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}";
		}
		const std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<unsigned char> buffer(size);
		if (!file.read(reinterpret_cast<std::istream::char_type*>(buffer.data()), size)) {
			mLength = 1.0f;
			std::string indexStr = std::to_string(index);
			return "vec2 midiSequencerGetNote0_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote1_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote2_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote3_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote4_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote5_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote6_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote7_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote8_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote9_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote10_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote11_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote12_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote13_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote14_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote15_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}";
		}
		file.close();
		std::streamsize pos = 0;
		if (buffer[pos++] != 'M' || buffer[pos++] != 'T' || buffer[pos++] != 'h' || buffer[pos++] != 'd') {
			mLength = 1.0f;
			std::string indexStr = std::to_string(index);
			return "vec2 midiSequencerGetNote0_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote1_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote2_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote3_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote4_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote5_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote6_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote7_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote8_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote9_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote10_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote11_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote12_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote13_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote14_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote15_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}";
		}
		pos += 4;
		auto format = static_cast<uint16_t>(buffer[pos++] << 8);
		format |= buffer[pos++];
		auto trackCount = static_cast<uint16_t>(buffer[pos++] << 8);
		trackCount |= buffer[pos++];
		auto timeDivision = static_cast<uint16_t>(buffer[pos++] << 8);
		timeDivision |= static_cast<uint16_t>(buffer[pos++]);
		float timeScale = 0.0f;
		if (timeDivision & 0x8000) {
			timeScale = 96000.0f / static_cast<float>(-static_cast<int8_t>(timeDivision & 0xFF00 >> 8) * timeDivision & 0xFF);
		}
		else {
			timeScale = 500000.0f / static_cast<float>(timeDivision & 0x7FFF) / 1000000.0f * 96000.0f;
		}
		if (buffer[pos++] != 'M' || buffer[pos++] != 'T' || buffer[pos++] != 'r' || buffer[pos++] != 'k') {
			mLength = 1.0f;
			std::string indexStr = std::to_string(index);
			return "vec2 midiSequencerGetNote0_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote1_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote2_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote3_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote4_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote5_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote6_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote7_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote8_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote9_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote10_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote11_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote12_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote13_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote14_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}vec2 midiSequencerGetNote15_" + indexStr + "(uint s) {return vec2(0.0f,0.0f);}";
		}
		auto length = static_cast<uint32_t>(buffer[pos++] << 24);
		length |= static_cast<uint32_t>(buffer[pos++] << 16);
		length |= static_cast<uint32_t>(buffer[pos++] << 8);
		length |= static_cast<uint32_t>(buffer[pos++]);
		uint32_t previousTime = 0;
		std::string functions[16];
		for (int i = 0; i < 16; i++) {
			functions[i] = "vec2 midiSequencerGetNote" + std::to_string(i) + "_" + std::to_string(index) + "(uint s) {\n";
		}
		std::tuple<uint32_t, float, uint8_t> playingNotes[16][128] = {};
		uint32_t trackEndPos = pos + length;
		uint8_t runningStatus = 0;
		std::vector<uint8_t> freeChannels = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		while (pos < trackEndPos) {
			uint32_t deltaTime = 0;
			for (uint8_t j = 0; j < 4; j++) {
				uint8_t value = static_cast<uint32_t>(buffer[pos++]);
				deltaTime <<= 7;
				deltaTime |= value & 0x7F;
				if (!(value & 0x80)) {
					break;
				}
			}
			previousTime += deltaTime;
			uint8_t status = 0;
			if (buffer[pos] & 0x80) {
				status = buffer[pos++];
				if (status < 0xF0) {
					runningStatus = status;
				}
			}
			else {
				status = runningStatus;
			}
			uint8_t channel = status & 0xF;
			if (uint8_t command = status & 0xF0; command == 0x80 || command == 0x90) {
				uint8_t note = buffer[pos++];
				if (uint8_t velocity = buffer[pos++]; velocity == 0 || command == 0x80) {
					auto [startTime, volume, c] = playingNotes[channel][note];
					playingNotes[channel][note] = {0, 0.0f, 0};
					freeChannels.push_back(c);
					if (volume > 0.0f) {
						const float frequency = 440.0f * std::pow(2.0f, (static_cast<float>(note) - 69.0f) / 12.0f);
						const float start = static_cast<float>(startTime) * timeScale;
						const float end = static_cast<float>(previousTime) * timeScale;
						functions[c] += "if(s>=" + std::to_string(start) + " && s<" + std::to_string(end) + ")return vec2(" + std::to_string(frequency) + "," + std::to_string(volume) + ");\n";
					}
				}
				else {
					playingNotes[channel][note] = {previousTime, static_cast<float>(velocity) / 127.0f, freeChannels.back()};
					freeChannels.pop_back();
				}
			}
		}
		std::string functionsStr;
		for (const std::string& function: functions) {
			functionsStr += function + "return vec2(0.0f,0.0f);}\n";
		}
		mLength = static_cast<float>(previousTime) * timeScale;
		return functionsStr;
	}

	std::string MidiSequencer::generateGLSL(const uint32_t index, std::unordered_map<std::string, std::string>&) {
		const std::string indexStr = std::to_string(index);
		std::string glsl;
		for (int i = 0; i < 16; i++) {
			std::string channelStr = std::to_string(i);
			glsl += "vec2 Data" + channelStr + "_" += indexStr + "=midiSequencerGetNote" += channelStr + "_" += indexStr + "(sample0%" + std::to_string(static_cast<uint32_t>(mLength)) + "u);\nfloat Frequency" += channelStr + "_" += indexStr + "=Data" += channelStr + "_" += indexStr + "[0];\nfloat Volume" += channelStr + "_" += indexStr + "=Data" += channelStr + "_" += indexStr + "[1];";
		}
		return glsl;
	}
}
