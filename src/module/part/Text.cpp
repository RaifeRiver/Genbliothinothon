#include "Text.h"

#include <utility>

namespace genbliothinothon::module::part {
	Text::Text(const glm::vec2 pos, const float size, std::string text, const glm::vec4 colour) : Part(pos, {size, 0.0f}), mSize(size), mText(std::move(text)), mColour(colour) {}

	void Text::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		renderer.text(pos, mSize, mText, mColour);
	}
}
