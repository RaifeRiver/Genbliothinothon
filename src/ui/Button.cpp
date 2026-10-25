#include "Button.h"

#include <utility>

namespace genbliothinothon::ui {
	Button::Button(std::string label) : mLabel(std::move(label)) {}

	Button& Button::setLabel(const std::string& label) {
		mLabel = label;
		return *this;
	}

	Button& Button::setFontSize(const float fontSize) {
		mFontSize = fontSize;
		return *this;
	}

	Button& Button::setColour(const glm::vec4 colour) {
		mColour = colour;
		return *this;
	}

	Button& Button::setBackgroundColour(const glm::vec4 colour) {
		mBackgroundColour = colour;
		return *this;
	}

	Button& Button::setPadding(const glm::vec4 padding) {
		mPadding = padding;
		return *this;
	}

	Button& Button::setCallback(const std::optional<std::function<void()>>& callback) {
		mCallback = callback;
		return *this;
	}

	void Button::onRelease() {
		Component::onRelease();
		if (mCallback.has_value()) {
			mCallback.value()();
		}
	}

	void Button::render(const glm::vec2 pos, render::UIRenderer& renderer) {
		glm::vec4 backgroundColour = mBackgroundColour;
		if (isPressed()) {
			backgroundColour = mBackgroundColour * 2.0f;
		}
		else if (isHovered()) {
			backgroundColour = mBackgroundColour * 1.5f;
		}
		const glm::vec2 size = calculateSize(renderer);
		renderer.rect(pos, size, backgroundColour);
		renderer.text(pos + glm::vec2{mPadding.x, mPadding.y}, mFontSize, mLabel, mColour);
	}

	glm::vec2 Button::calculateSize(render::UIRenderer& renderer) {
		return renderer.getTextSize(mFontSize, mLabel) + glm::vec2{mPadding.x + mPadding.z, mPadding.y + mPadding.w};
	}
}
