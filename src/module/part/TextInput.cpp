#include "TextInput.h"

namespace genbliothinothon::module::part {
	TextInput::TextInput(Module* parent, const glm::vec2& pos, const glm::vec2& size) : Part(parent, pos, size) {}

	void TextInput::setText(const std::string& text) {
		mText = text;
	}

	void TextInput::click(glm::vec2) {
		mSelected = true;
		mCursorTime = std::chrono::steady_clock::now();
	}

	void TextInput::clickOff(glm::vec2) {
		mSelected = false;
	}

	void TextInput::charPressed(const char key) {
		if (mSelected) {
			mText += key;
		}
	}

	void TextInput::keyPressed(const int key) {
		if (mSelected) {
			if (key == GLFW_KEY_BACKSPACE && !mText.empty()) {
				mText.pop_back();
			}
		}
	}

	void TextInput::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rect(pos, size, glm::vec4{0.06f, 0.06f, 0.11f, 1.0f});
		const float textHeight = size.y / 2.0f - 7.0f;
		renderer.text(pos + glm::vec2{2.0f, textHeight}, 14.0f, mText, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
		if (mSelected) {
			if (const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now(); std::chrono::duration<float>(now - mCursorTime).count() < 0.5f) {
				renderer.rect(pos + glm::vec2{renderer.getTextWidth(14.0f, mText) + 2.0f, textHeight}, glm::vec2{2.0f, 14.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
			}
			else if (std::chrono::duration<float>(now - mCursorTime).count() >= 1.0f) {
				mCursorTime = now;
			}
		}
	}
}
