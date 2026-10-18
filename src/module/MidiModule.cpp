#include "MidiModule.h"

#include <cmath>
#include <sstream>

#include "part/Background.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	MidiModule::MidiModule(const glm::vec2 pos) : Module(pos, glm::vec2{200.0f, 150.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 150.0f}, "Midi player"));
		addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 46.0f}, glm::vec2{194.0f, 20.0f}));
		//addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 69.0f}, glm::vec2{194.0f, 20.0f}));
	}

	void MidiModule::setInputCSV(const std::string& inputCSV) {
		mInputCSV = inputCSV;
		setDirty();
	}

	std::string MidiModule::generateFunctions(const uint32_t index) {
		mLength = 0.0f;
		std::string functions = "vec2 midiGetNote" + std::to_string(index) + "(uint s) {";
		std::string line;
		constexpr float beatLength = 60.0f * 96000.0f / 112.0f;
		std::istringstream input(mInputCSV);
		while (std::getline(input, line)) {
			if (line.starts_with("//")) {
				continue;
			}
			std::stringstream stream(line);
			std::string cell;
			std::vector<std::string> row;
			while (std::getline(stream, cell, ',')) {
				row.push_back(cell);
			}
			if (row.size() >= 7) {
				const int bar = std::stoi(row[0]);
				const float beat = std::stof(row[1]);
				const float length = std::stof(row[4]);
				const float start = static_cast<float>(bar) * 7.0f + beat;
				uint32_t semitone = 0;
				switch (row[2][0]) {
					case 'a':
						semitone = 0;
						break;
					case 'b':
						semitone = 2;
						break;
					case 'c':
						semitone = 3;
						break;
					case 'd':
						semitone = 5;
						break;
					case 'e':
						semitone = 7;
						break;
					case 'f':
						semitone = 8;
						break;
					case 'g':
						semitone = 10;
						break;
					default:
						throw std::runtime_error("Invalid note");
				}
				const float frequency = 27.5f * std::pow(2.0f, (static_cast<float>(semitone) + static_cast<float>(std::stoi(row[3])) * 12.0f - 1.0f) / 12.0f);
				functions += "if(s>=" + std::to_string(start * beatLength) + " && s<" + std::to_string((start + length) * beatLength) + ")return vec2(" + std::to_string(frequency) + "," + std::to_string(std::stoi(row[6])) + ");\n";
				mLength = (start + length) * beatLength;
			}
		}
		functions += "return vec2(0.0f,0.0f);}";
		return functions;
	}

	std::string MidiModule::generateGLSL(const uint32_t index, std::unordered_map<std::string, std::string>& inputValues) {
		return "midiGetNote" + std::to_string(index) + "(sample0%" + std::to_string(static_cast<uint32_t>(mLength)) + "u)[0]";
	}
}
