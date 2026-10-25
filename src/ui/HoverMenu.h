#pragma once

#include "Component.h"

namespace genbliothinothon::ui {
	class HoverMenu : public Component {
	public:
		explicit HoverMenu(std::string label);

		[[nodiscard]] std::string& getLabel() {
			return mLabel;
		}

		HoverMenu& setLabel(const std::string& label);

		[[nodiscard]] float& getFontSize() {
			return mFontSize;
		}

		HoverMenu& setFontSize(float fontSize);

		[[nodiscard]] glm::vec4& getColour() {
			return mColour;
		}

		HoverMenu& setColour(glm::vec4 colour);

		[[nodiscard]] glm::vec4& getBackgroundColour() {
			return mBackgroundColour;
		}

		HoverMenu& setBackgroundColour(glm::vec4 colour);

		[[nodiscard]] glm::vec4& getPadding() {
			return mPadding;
		}

		HoverMenu& setPadding(glm::vec4 padding);

		[[nodiscard]] Component* getMenu() const {
			return mMenu.get();
		}

		HoverMenu& setMenu(std::unique_ptr<Component> menu);

		void onHover() override;

		void onPress(glm::vec2 pos) override;

		void onRelease() override;

		void onMouseMove(glm::vec2 pos) override;

		void render(glm::vec2 pos, render::UIRenderer& renderer) override;

		[[nodiscard]] glm::vec2 calculateSize(render::UIRenderer& renderer) override;

		[[nodiscard]] glm::vec4 calculateInteractionSize(render::UIRenderer& renderer) override;

	private:
		std::string mLabel;
		float mFontSize = 14.0f;
		glm::vec4 mColour = {1.0f, 1.0f, 1.0f, 1.0f};
		glm::vec4 mBackgroundColour = {0.08f, 0.08f, 0.14f, 1.0f};
		glm::vec4 mPadding = {3.0f, 3.0f, 3.0f, 3.0f};
		std::unique_ptr<Component> mMenu = nullptr;
		bool mOpen = false;
	};
}
