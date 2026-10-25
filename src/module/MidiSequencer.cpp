#include "MidiSequencer.h"

#include <cmath>
#include <deque>
#include <fstream>

#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	MidiSequencer::MidiSequencer(const glm::vec2& pos) : Module(pos, {200.0f, 139.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 139.0f}, "MIDI Sequencer"));
		addPart(std::make_unique<part::RemoveButton>(this));
		mTextInput = &addPart(std::make_unique<part::TextInput>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 47.0f}, glm::vec2{194.0f, 20.0f}, "Frequency", 16));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 70.0f}, glm::vec2{194.0f, 20.0f}, "Volume", 16));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 93.0f}, glm::vec2{194.0f, 20.0f}, "NoteStart", 16));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 116.0f}, glm::vec2{194.0f, 20.0f}, "NoteHeld", 16));
	}

	MidiSequencer::MidiSequencer(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		const std::vector<std::unique_ptr<part::Part>>& parts = getParts();
		if (version == 0) {
			setSize({200.0f, 139.0f});
			parts[0]->setSize({200.0f, 139.0f});
			addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 116.0f}, glm::vec2{194.0f, 20.0f}, "NoteHeld", 16));
		}
		mTextInput = reinterpret_cast<part::TextInput*>(parts[2].get());
	}

	std::string MidiSequencer::generateFunctions(const uint32_t index) {
		std::ifstream file(mTextInput->getText(), std::ios::binary | std::ios::ate);
		if (!file.is_open()) {
			mLength = 1.0f;
			std::string indexStr = std::to_string(index);
			return "vec3 midiSequencerGetNote0_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote1_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote2_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote3_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote4_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote5_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote6_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote7_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote8_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote9_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote10_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote11_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote12_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote13_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote14_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote15_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}";
		}
		const std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<unsigned char> buffer(size);
		if (!file.read(reinterpret_cast<std::istream::char_type*>(buffer.data()), size)) {
			mLength = 1.0f;
			std::string indexStr = std::to_string(index);
			return "vec3 midiSequencerGetNote0_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote1_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote2_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote3_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote4_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote5_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote6_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote7_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote8_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote9_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote10_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote11_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote12_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote13_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote14_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote15_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}";
		}
		file.close();
		std::streamsize pos = 0;
		if (buffer[pos++] != 'M' || buffer[pos++] != 'T' || buffer[pos++] != 'h' || buffer[pos++] != 'd') {
			mLength = 1.0f;
			std::string indexStr = std::to_string(index);
			return "vec3 midiSequencerGetNote0_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote1_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote2_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote3_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote4_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote5_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote6_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote7_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote8_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote9_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote10_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote11_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote12_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote13_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote14_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote15_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}";
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
			timeScale = 96000.0f / static_cast<float>(-static_cast<int8_t>((timeDivision & 0xFF00) >> 8) * (timeDivision & 0xFF));
		}
		else {
			timeScale = 500000.0f / static_cast<float>(timeDivision & 0x7FFF) / 1000000.0f * 96000.0f;
		}
		if (buffer[pos++] != 'M' || buffer[pos++] != 'T' || buffer[pos++] != 'r' || buffer[pos++] != 'k') {
			mLength = 1.0f;
			std::string indexStr = std::to_string(index);
			return "vec3 midiSequencerGetNote0_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote1_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote2_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote3_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote4_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote5_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote6_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote7_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote8_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote9_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote10_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote11_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote12_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote13_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote14_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}vec3 midiSequencerGetNote15_" + indexStr + "(uint s) {return vec3(0.0f,0.0f,0.0f);}";
		}
		auto length = static_cast<uint32_t>(buffer[pos++] << 24);
		length |= static_cast<uint32_t>(buffer[pos++] << 16);
		length |= static_cast<uint32_t>(buffer[pos++] << 8);
		length |= static_cast<uint32_t>(buffer[pos++]);
		uint32_t previousTime = 0;
		std::string functions[16];
		for (int i = 0; i < 16; i++) {
			functions[i] = "vec3 midiSequencerGetNote" + std::to_string(i) + "_" + std::to_string(index) + "(uint s) {\n";
		}
		std::tuple<uint32_t, float, uint8_t> playingNotes[16][128] = {};
		uint32_t trackEndPos = pos + length;
		uint8_t runningStatus = 0;
		std::deque<uint8_t> freeChannels = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		while (pos < trackEndPos) {
			uint32_t deltaTime = parseVariableInt(pos, buffer.data());
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
						std::string startStr = std::to_string(start);
						functions[c] += "if(s>=" + startStr + "&&s<" + std::to_string(end) + ")return vec3(" + std::to_string(frequency) + "," + std::to_string(volume) + "," += startStr + ");\n";
					}
				}
				else {
					playingNotes[channel][note] = {previousTime, static_cast<float>(velocity) / 127.0f, freeChannels.front()};
					freeChannels.pop_front();
				}
			}
			else if (command >= 0xA0 && command <= 0xAF) {
				// TODO: Polyphonic key pressure command
				pos += 2;
			}
			else if (command >= 0xB0 && command <= 0xBF) {
				// TODO: Control change command
				pos += 2;
			}
			else if (command >= 0xC0 && command <= 0xCF) {
				// TODO: Program change command
				pos++;
			}
			else if (command >= 0xD0 && command <= 0xDF) {
				// TODO: Channel pressure command
				pos++;
			}
			else if (command >= 0xE0 && command <= 0xEF) {
				// TODO: Pitch bend command
				pos += 2;
			}
			else if (command == 0xF0 || command == 0xF7) {
				pos += parseVariableInt(pos, buffer.data());
			}
			else if (command == 0xFF) {
				// TODO: Meta command
				if (uint8_t type = buffer[pos++]; type == 0x51) {
					uint32_t value = buffer[pos++] << 16;
					value |= buffer[pos++] << 8;
					value |= buffer[pos++];
					if (!(timeDivision & 0x8000)) {
						timeScale = 500000.0f / static_cast<float>(value) / 1000000.0f * 96000.0f / static_cast<float>(timeDivision & 0x7FFF);
					}
				}
				else {
					pos += parseVariableInt(pos, buffer.data());
				}
			}
		}
		std::string functionsStr;
		for (const std::string& function: functions) {
			functionsStr += function + "return vec3(0.0f,0.0f,0.0f);}\n";
		}
		mLength = static_cast<float>(previousTime) * timeScale;
		return functionsStr;
	}

	std::string MidiSequencer::generateGLSL(const uint32_t index, std::unordered_map<std::string, std::string>&) {
		const std::string indexStr = std::to_string(index);
		std::string glsl;
		for (int i = 0; i < 16; i++) {
			std::string channelStr = std::to_string(i);
			glsl += "vec3 Data" + channelStr + "_" += indexStr + "=midiSequencerGetNote" += channelStr + "_" += indexStr + "(sample0%" + std::to_string(static_cast<uint32_t>(mLength)) + "u);\nfloat Frequency" += channelStr + "_" += indexStr + "=Data" += channelStr + "_" += indexStr + ".x;\nfloat Volume" += channelStr + "_" += indexStr + "=Data" += channelStr + "_" += indexStr + ".y;\nuint NoteStart" += channelStr + "_" += indexStr + "=uint(Data" += channelStr + "_" += indexStr + ".z);\nbool NoteHeld" += channelStr + "_" += indexStr + "=Data" += channelStr + "_" += indexStr + ".y>0.0f;\n";
		}
		return glsl;
	}

	uint32_t MidiSequencer::parseVariableInt(std::streamsize& pos, const uint8_t* buffer) {
		uint32_t value = 0;
		for (uint8_t i = 0; i < 4; i++) {
			const uint8_t b = static_cast<uint32_t>(buffer[pos++]);
			value <<= 7;
			value |= b & 0x7F;
			if (!(b & 0x80)) {
				break;
			}
		}
		return value;
	}
}
