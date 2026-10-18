#include "Output.h"

#include "part/Output.h"
#include "part/Background.h"

namespace genbliothinothon::module {
	Output::Output(const glm::vec2& pos) : Module(pos, glm::vec2{200.0f, 100.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 100.0f}, "Output"));
		addPart(std::make_unique<part::Output>(this, glm::vec2{3.0f, 25.0f}, glm::vec2{30.0f, 30.0f}, "Left"));
		addPart(std::make_unique<part::Output>(this, glm::vec2{36.0f, 25.0f}, glm::vec2{30.0f, 30.0f}, "Right"));
	}

	std::string Output::generateGLSL(std::unordered_map<std::string, std::string>& inputValues) {
		return inputValues["Left"];
	}
}
