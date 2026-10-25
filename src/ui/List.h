#pragma once

#include <optional>
#include <vector>

#include "Component.h"

namespace genbliothinothon::ui {
	class List : public Component {
	public:
		explicit List(const std::vector<std::string>& items);

		[[nodiscard]] std::vector<std::string>& getItems() {
			return mItems;
		}

		List& setItems(const std::vector<std::string>& items);

		[[nodiscard]] uint32_t& getSelected() {
			return mSelected;
		}

		List& setSelected(uint32_t selected);

		[[nodiscard]] float getFontSize() const {
			return mFontSize;
		}

		List& setFontSize(float fontSize);

		[[nodiscard]] glm::vec4& getColour() {
			return mColour;
		}

		List& setColour(glm::vec4 colour);

		[[nodiscard]] glm::vec4& getBackgroundColour() {
			return mBackgroundColour;
		}

		List& setBackgroundColour(glm::vec4 colour);

		[[nodiscard]] glm::vec4& getPadding() {
			return mPadding;
		}

		List& setPadding(glm::vec4 padding);

		[[nodiscard]] float& getItemPadding() {
			return mItemPadding;
		}

		List& setItemPadding(float padding);

		[[nodiscard]] std::optional<std::function<void(uint32_t)>>& getSelectCallback() {
			return mSelectCallback;
		}

		List& setSelectCallback(const std::optional<std::function<void(uint32_t)>>& callback);

		void onHoverStop() override;

		void onPress(glm::vec2 pos) override;

		void onRelease() override;

		void onMouseMove(glm::vec2 pos) override;

		void render(glm::vec2 pos, render::UIRenderer& renderer) override;

		[[nodiscard]] glm::vec2 calculateSize(render::UIRenderer& renderer) override;

	private:
		std::vector<std::string> mItems;
		uint32_t mSelected = 0;
		float mFontSize = 14.0f;
		glm::vec4 mColour = {1.0f, 1.0f, 1.0f, 1.0f};
		glm::vec4 mBackgroundColour = {0.08f, 0.08f, 0.14f, 1.0f};
		glm::vec4 mPadding = {3.0f, 1.5f, 3.0f, 1.5f};
		float mItemPadding = 3.0f;
		std::optional<std::function<void(uint32_t)>> mSelectCallback;
		uint32_t mHovered = UINT32_MAX;
		uint32_t mPressed = UINT32_MAX;
	};
}
