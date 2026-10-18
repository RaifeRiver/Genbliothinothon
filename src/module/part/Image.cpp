#include "Image.h"

#include <utility>

namespace genbliothinothon::module::part {
	Image::Image(Module* parent, const glm::vec2 pos, const glm::vec2 size, std::string texture, const glm::vec4 tint) : Part(parent, pos, size), mTexture(std::move(texture)), mTint(tint) {}

	void Image::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		renderer.rect(pos, getSize(), mTint, mTexture);
	}
}
