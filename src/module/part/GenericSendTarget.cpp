#include "GenericSendTarget.h"

namespace genbliothinothon::module::part {
	GenericSendTarget::GenericSendTarget(Module* parent, const glm::vec2 pos, const glm::vec2 size, const std::string& name, const std::string& defaultValue, const bool multiChannel) : SendTarget(parent, pos, size, name, multiChannel), mDefaultValue(defaultValue) {}

	void GenericSendTarget::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rect(pos, size, glm::vec4{0.06f, 0.06f, 0.11f, 1.0f});
		const float textHeight = size.y / 2.0f - 7.0f;
		renderer.text(pos + glm::vec2{2.0f, textHeight}, 14.0f, getName(), glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, size.x);
	}
}
