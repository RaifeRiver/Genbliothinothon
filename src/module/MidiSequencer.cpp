#include "MidiSequencer.h"

#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>

#include "part/Background.h"
#include "part/RemoveButton.h"

namespace genbliothinothon::module {
	MidiSequencer::MidiSequencer(const glm::vec2& pos) : Module(pos, {200.0f, 139.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 139.0f}, "MIDI Sequencer"));
		addPart(std::make_unique<part::RemoveButton>(this));
		mTextInput = &addPart(std::make_unique<part::TextInput>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}));
		mTextInput->setSetCallback([this](const std::string& path) {
			loadMidiFile(path);
		});
		mFrequency = &addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 47.0f}, glm::vec2{194.0f, 20.0f}, "Frequency", "float", 1));
		mVolume = &addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 70.0f}, glm::vec2{194.0f, 20.0f}, "Volume", "float", 1));
		mNoteStart = &addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 93.0f}, glm::vec2{194.0f, 20.0f}, "NoteStart", "float", 1));
		mNoteHeld = &addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 116.0f}, glm::vec2{194.0f, 20.0f}, "NoteHeld", "float", 1));
	}

	MidiSequencer::MidiSequencer(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		const std::vector<std::unique_ptr<part::Part>>& parts = getParts();
		mTextInput = reinterpret_cast<part::TextInput*>(parts[2].get());
		mTextInput->setSetCallback([this](const std::string& path) {
			loadMidiFile(path);
		});
		mFrequency = reinterpret_cast<part::Send*>(parts[3].get());
		mVolume = reinterpret_cast<part::Send*>(parts[4].get());
		mNoteStart = reinterpret_cast<part::Send*>(parts[5].get());
		mNoteHeld = reinterpret_cast<part::Send*>(parts[6].get());
	}

	std::string MidiSequencer::generateFunctions(const uint32_t index) {
		const std::string indexStr = std::to_string(index);
		if (mLength == -1.0f) {
			loadMidiFile(mTextInput->getText());
		}
		if (mChannelCount == 0) {
			return "vec4 midiSequencerGetNote0_" + indexStr + "(uint s) {return vec4(0.0f,0.0f,0.0f,0.0f);}";
		}
		std::string functions = mFunctions;
		size_t pos = 0;
		while ((pos = functions.find("$index", pos)) != std::string::npos) {
			functions.replace(pos, 6, indexStr);
			pos += indexStr.length();
		}
		return functions;
	}

	std::string MidiSequencer::generateGLSL(const std::string send) {
		std::string extension;
		if (send == "Frequency") {
			extension = "x";
		}
		else if (send == "Volume") {
			extension = "y";
		}
		else if (send == "NoteStart") {
			extension = "z";
		}
		else {
			extension = "w==1.0f";
		}
		return "midiSequencerGetNote$Channel_$Index($Sample)." + extension;
	}

	void MidiSequencer::setChannelCounts() const {
		std::cout << "Setting midi channel counts to " << mChannelCount << std::endl;
		uint32_t channelCount = std::max(mChannelCount, 1u);
		channelCount = 1;
		mFrequency->setChannelCount(channelCount);
		mVolume->setChannelCount(channelCount);
		mNoteStart->setChannelCount(channelCount);
		mNoteHeld->setChannelCount(channelCount);
	}

	void MidiSequencer::loadMidiFile(const std::string& path) {
		std::cout << "Loading midi file " << path << std::endl;
		mChannelCount = 0;
		mFunctions = "";
		mLength = 1.0f;
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		if (!file.is_open()) {
			setChannelCounts();
			return;
		}
		const std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<unsigned char> buffer(size);
		if (!file.read(reinterpret_cast<std::istream::char_type*>(buffer.data()), size)) {
			setChannelCounts();
			return;
		}
		file.close();
		std::streamsize pos = 0;
		if (buffer[pos++] != 'M' || buffer[pos++] != 'T' || buffer[pos++] != 'h' || buffer[pos++] != 'd') {
			setChannelCounts();
			return;
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
			setChannelCounts();
			return;
		}
		auto length = static_cast<uint32_t>(buffer[pos++] << 24);
		length |= static_cast<uint32_t>(buffer[pos++] << 16);
		length |= static_cast<uint32_t>(buffer[pos++] << 8);
		length |= static_cast<uint32_t>(buffer[pos++]);
		uint32_t previousTime = 0;
		std::vector<std::string> functions;
		std::tuple<uint32_t, float, uint32_t> playingNotes[16][128] = {};
		uint32_t trackEndPos = pos + length;
		uint8_t runningStatus = 0;
		std::deque<uint32_t> freeChannels = {};
		std::deque<std::pair<uint32_t, float>> cooldownChannels = {};
		uint32_t nextChannel = 0;
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
					if (volume > 0.0f) {
						const float frequency = 440.0f * std::pow(2.0f, (static_cast<float>(note) - 69.0f) / 12.0f);
						const float start = static_cast<float>(startTime) * timeScale;
						const float end = static_cast<float>(previousTime) * timeScale;
						cooldownChannels.emplace_back(c, end + RELEASE_TIME);
						std::string startStr = std::to_string(start);
						functions[c] += "if(s>=" + startStr + "&&s<" + std::to_string(end + RELEASE_TIME) + ")return vec4(" + std::to_string(frequency) + "," + std::to_string(volume) + "," += startStr + ",s>=" + std::to_string(end) + "?0.0f:1.0f);\n";
					}
				}
				else {
					if (freeChannels.empty()) {
						if (!cooldownChannels.empty() && cooldownChannels.front().second < static_cast<float>(previousTime) * timeScale) {
							freeChannels.push_back(cooldownChannels.front().first);
							cooldownChannels.pop_front();
						}
						else {
							functions.emplace_back("vec4 midiSequencerGetNote" + std::to_string(nextChannel) +"_$index(uint s) {\n");
							freeChannels.push_back(nextChannel++);
						}
					}
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
		mChannelCount = nextChannel;
		setChannelCounts();
		for (const std::string& function: functions) {
			mFunctions += function + "return vec4(0.0f,0.0f,0.0f,0.0f);}\n";
		}
		mLength = static_cast<float>(previousTime) * timeScale;
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
