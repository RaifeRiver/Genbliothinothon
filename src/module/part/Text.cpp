#include "Text.h"

namespace genbliothinothon::module::part {
	Text::Text(const glm::vec2 pos, const float size, const std::string& text, const glm::vec4 colour) : Part(pos), mSize(size), mText(text), mColour(colour) {}

	void Text::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		renderer.text(pos, mSize, mText, mColour);
	}
}
