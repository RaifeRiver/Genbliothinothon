#include "Output.h"

#include "part/Output.h"
#include "part/Background.h"

namespace genbliothinothon::module {
	Output::Output(const glm::vec2& pos) : Module(pos, glm::vec2{130.0f, 80.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{130.0f, 80.0f}, "Output"));
		addPart(std::make_unique<part::Output>(this, glm::vec2{5.0f, 25.0f}, glm::vec2{50.0f, 50.0f}, "Left")).setCombiningMode(part::CombiningMode::SUM);
		addPart(std::make_unique<part::Output>(this, glm::vec2{75.0f, 25.0f}, glm::vec2{50.0f, 50.0f}, "Right")).setCombiningMode(part::CombiningMode::SUM);
	}

	Output::Output(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {}

	std::string Output::generateGLSL(uint32_t, std::unordered_map<std::string, std::string>& inputValues) {
		const auto leftIt = inputValues.find("Left0_");
		const auto rightIt = inputValues.find("Right0_");
		std::string left = "0.0f";
		std::string right = "0.0f";
		if (leftIt != inputValues.end()) {
			left = leftIt->second;
		}
		if (rightIt != inputValues.end()) {
			right = rightIt->second;
		}
		return "float leftAudio=" + left + ";\nfloat rightAudio=" + right + ";\n";
	}
}
