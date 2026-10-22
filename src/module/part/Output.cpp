#include "Output.h"

namespace genbliothinothon::module::part {
	Output::Output(Module* parent, const glm::vec2& pos, const glm::vec2& size, const std::string& name) : SendTarget(sType, parent, pos, size, name) {}

	Output::Output(const uint32_t version, util::FileIStream& stream) : SendTarget(version, stream) {}

	std::string Output::getDefaultValueString() {
		return "0.0f";
	}

	void Output::setType(const uint32_t type) {
		sType = type;
	}

	void Output::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rect(pos, size, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
}
