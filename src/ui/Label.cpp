#include "Label.h"

namespace genbliothinothon::ui {
	Label::Label(const std::string& text) : mText(text) {}

	void Label::setText(const std::string& text) {
		mText = text;
	}

	void Label::setFontSize(const float fontSize) {
		mFontSize = fontSize;
	}

	void Label::setColour(const glm::vec4 colour) {
		mColour = colour;
	}

	void Label::setPadding(const glm::vec4 padding) {
		mPadding = padding;
	}

	void Label::render(const glm::vec2 pos, render::UIRenderer& renderer) {
		renderer.text({pos.x + mPadding.x, pos.y + mPadding.y}, mFontSize, mText, mColour);
	}

	glm::vec2 Label::calculateSize(render::UIRenderer& renderer) {
		const glm::vec2 textSize = renderer.getTextSize(mFontSize, mText);
		return {textSize.x + mPadding.x + mPadding.z, textSize.y + mPadding.y + mPadding.w};
	}
}
