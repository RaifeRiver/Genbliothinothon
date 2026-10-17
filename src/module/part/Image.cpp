#include "Image.h"

#include <utility>

namespace genbliothinothon::module::part {
	Image::Image(const glm::vec2 pos, const glm::vec2 size, std::string texture, const glm::vec4 tint) : Part(pos), mSize(size), mTexture(std::move(texture)), mTint(tint) {}

	void Image::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		renderer.rect(pos, mSize, mTint, mTexture);
	}
}
