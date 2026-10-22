#include "RemoveButton.h"

#include "module/Module.h"

namespace genbliothinothon::module::part {
	RemoveButton::RemoveButton(Module* parent) : Part(sType, parent, glm::vec2{parent->getSize().x - 16.0f, 0.0f}, {16.0f, 16.0f}) {}

	RemoveButton::RemoveButton(const uint32_t version, util::FileIStream& stream) : Part(version, stream) {}

	void RemoveButton::setType(const uint32_t type) {
		sType = type;
	}

	void RemoveButton::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		renderer.rect(pos, glm::vec2{16.0f, 16.0f}, glm::vec4{0.1f, 0.1f, 0.18f, 1.0f});
		renderer.centredText(pos + glm::vec2{8.0f, 0.0f}, 16.0f, "X", glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
	}
}
