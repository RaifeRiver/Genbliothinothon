#pragma once

#include <optional>

#include "Component.h"

namespace genbliothinothon::ui {
	class Button : public Component {
	public:
		explicit Button(std::string label);

		[[nodiscard]] std::string& getLabel() {
			return mLabel;
		}

		Button& setLabel(const std::string& label);

		[[nodiscard]] float& getFontSize() {
			return mFontSize;
		}

		Button& setFontSize(float fontSize);

		[[nodiscard]] glm::vec4& getColour() {
			return mColour;
		}

		Button& setColour(glm::vec4 colour);

		[[nodiscard]] glm::vec4& getBackgroundColour() {
			return mBackgroundColour;
		}

		Button& setBackgroundColour(glm::vec4 colour);

		[[nodiscard]] glm::vec4& getPadding() {
			return mPadding;
		}

		Button& setPadding(glm::vec4 padding);

		[[nodiscard]] std::optional<std::function<void()>>& getCallback() {
			return mCallback;
		}

		Button& setCallback(const std::optional<std::function<void()>>& callback);

		void onRelease() override;

		void render(glm::vec2 pos, render::UIRenderer& renderer) override;

		[[nodiscard]] glm::vec2 calculateSize(render::UIRenderer& renderer) override;

	private:
		std::string mLabel;
		float mFontSize = 14.0f;
		glm::vec4 mColour = {1.0f, 1.0f, 1.0f, 1.0f};
		glm::vec4 mBackgroundColour = {0.08f, 0.08f, 0.14f, 1.0f};
		glm::vec4 mPadding = {3.0f, 3.0f, 3.0f, 3.0f};
		std::optional<std::function<void()>> mCallback;
	};
}
