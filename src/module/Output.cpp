#include "Output.h"

#include "part/Output.h"
#include "part/Background.h"

namespace genbliothinothon::module {
	Output::Output(const glm::vec2& pos) : Module(pos, glm::vec2{130.0f, 80.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{130.0f, 80.0f}, "Output"));
		addPart(std::make_unique<part::Output>(this, glm::vec2{5.0f, 25.0f}, glm::vec2{50.0f, 50.0f}, "Left"));
		addPart(std::make_unique<part::Output>(this, glm::vec2{75.0f, 25.0f}, glm::vec2{50.0f, 50.0f}, "Right"));
	}

	std::string Output::generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) {
		return "float leftAudio=" + inputValues["Left"] + ";\nfloat rightAudio=" + inputValues["Right"] + ";\n";
	}
}
