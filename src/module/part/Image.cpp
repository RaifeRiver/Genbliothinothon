#include "Image.h"

#include <utility>

namespace genbliothinothon::module::part {
	Image::Image(Module* parent, const glm::vec2 pos, const glm::vec2 size, std::string texture, const glm::vec4 tint) : Part(sType, parent, pos, size), mTexture(std::move(texture)), mTint(tint) {}

	Image::Image(const uint32_t version, util::FileIStream& stream) : Part(version, stream) {
		stream >> mTexture;
		stream >> mTint;
	}

	void Image::save(util::FileOStream& stream) {
		Part::save(stream);
		stream << mTexture;
		stream << mTint;
	}

	void Image::setType(const uint32_t type) {
		sType = type;
	}

	void Image::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		renderer.rect(pos, getSize(), mTint, mTexture);
	}
}
