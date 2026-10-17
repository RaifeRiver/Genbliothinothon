#include "Background.h"

#include <utility>

namespace genbliothinothon::module::part {
	Background::Background(const glm::vec2 pos, const glm::vec2 size, std::string name) : Part(pos), mSize(size), mName(std::move(name)) {}

	void Background::setSize(const glm::vec2 size) {
		mSize = size;
	}

	void Background::setName(const std::string& name) {
		mName = name;
	}

	void Background::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		renderer.rect(pos, mSize, glm::vec4{0.08f, 0.08f, 0.13f, 1.0f}, "module_background");
		renderer.text(pos + glm::vec2{4.0f, 2.0f}, 14.0f, mName, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
	}
}
