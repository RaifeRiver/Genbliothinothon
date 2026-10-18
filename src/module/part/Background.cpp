#include "Background.h"

#include <utility>

namespace genbliothinothon::module::part {
	Background::Background(Module* parent, const glm::vec2 pos, const glm::vec2 size, std::string name) : Part(parent, pos, size), mName(std::move(name)) {}

	void Background::setName(const std::string& name) {
		mName = name;
	}

	void Background::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		renderer.rect(pos, getSize(), glm::vec4{0.08f, 0.08f, 0.14f, 1.0f}, "module_background");
		renderer.text(pos + glm::vec2{4.0f, 2.0f}, 18.0f, mName, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
	}
}
