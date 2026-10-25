#pragma once

#include "Component.h"

namespace genbliothinothon::ui {
	class Label : public Component {
	public:
		explicit Label(const std::string& text);

		[[nodiscard]] std::string& getText() {
			return mText;
		}

		void setText(const std::string& text);

		[[nodiscard]] float& getFontSize() {
			return mFontSize;
		}

		void setFontSize(float fontSize);

		[[nodiscard]] glm::vec4& getColour() {
			return mColour;
		}

		void setColour(glm::vec4 colour);

		[[nodiscard]] glm::vec4& getPadding() {
			return mPadding;
		}

		void setPadding(glm::vec4 padding);

		void render(glm::vec2 pos, render::UIRenderer& renderer) override;

		[[nodiscard]] glm::vec2 calculateSize(render::UIRenderer& renderer) override;

	private:
		std::string mText;
		float mFontSize = 14.0f;
		glm::vec4 mColour = {1.0f, 1.0f, 1.0f, 1.0f};
		glm::vec4 mPadding = {0.0f, 0.0f, 0.0f, 0.0f};
	};
}
