#include "Text.h"

#include <utility>

namespace genbliothinothon::module::part {
	Text::Text(Module* parent, const glm::vec2 pos, const float size, std::string text, const glm::vec4 colour) : Part(sType, parent, pos, {size, 0.0f}), mSize(size), mText(std::move(text)), mColour(colour) {}

	Text::Text(const uint32_t version, util::FileIStream& stream) : Part(version, stream) {
		stream >> mSize;
		stream >> mText;
		stream >> mColour;
	}

	void Text::save(util::FileOStream& stream) {
		Part::save(stream);
		stream << mSize;
		stream << mText;
		stream << mColour;
	}

	void Text::setType(const uint32_t type) {
		sType = type;
	}

	void Text::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		renderer.text(pos, mSize, mText, mColour);
	}
}
