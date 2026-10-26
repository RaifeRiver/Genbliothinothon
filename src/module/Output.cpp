#include "Output.h"

#include "Genbliothinothon.h"
#include "ModuleManager.h"
#include "part/Background.h"

namespace genbliothinothon::module {
	Output::Output(const glm::vec2& pos) : Module(pos, glm::vec2{130.0f, 80.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{130.0f, 80.0f}, "Output"));
		left = &addPart(std::make_unique<part::Output>(this, glm::vec2{5.0f, 25.0f}, glm::vec2{50.0f, 50.0f}, "Left"));
		left->setCombiningMode(part::CombiningMode::SUM);
		right = &addPart(std::make_unique<part::Output>(this, glm::vec2{75.0f, 25.0f}, glm::vec2{50.0f, 50.0f}, "Right"));
		right->setCombiningMode(part::CombiningMode::SUM);
	}

	Output::Output(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		const std::vector<std::unique_ptr<part::Part>>& parts = getParts();
		left = reinterpret_cast<part::Output*>(parts[1].get());
		right = reinterpret_cast<part::Output*>(parts[2].get());
	}

	std::string Output::generateGLSL(const std::string send) {
		if (send == "Left") {
			return "$Left";
		}
		return "$Right";
	}
}
