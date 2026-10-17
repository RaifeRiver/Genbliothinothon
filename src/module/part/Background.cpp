#include "Background.h"

namespace genbliothinothon::module::part {
	Background::Background(const glm::vec2 pos, const glm::vec2 size) : Part(pos), mSize(size) {}

	void Background::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		renderer.rect(pos, mSize, glm::vec4{0.0f, 0.0f, 0.0f, 1.0f});
	}
}
