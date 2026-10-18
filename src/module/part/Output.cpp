#include "Output.h"

namespace genbliothinothon::module::part {
	Output::Output(Module* parent, const glm::vec2& pos, const glm::vec2& size, const std::string& name) : SendTarget(parent, pos, size, name) {}

	std::string Output::getDefaultValueString() {
		return "0.0f";
	}

	void Output::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rect(pos, size, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
}
