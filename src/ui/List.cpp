#include "List.h"

#include <iostream>

#include "Genbliothinothon.h"

namespace genbliothinothon::ui {
	List::List(const std::vector<std::string>& items) : mItems(items) {}

	List& List::setItems(const std::vector<std::string>& items) {
		mItems = items;
		return *this;
	}

	List& List::setSelected(uint32_t selected) {
		if (selected >= mItems.size()) {
			selected = mItems.size() - 1;
		}
		mSelected = selected;
		if (mSelectCallback.has_value()) {
			mSelectCallback.value()(mSelected);
		}
		return *this;
	}

	List& List::setFontSize(const float fontSize) {
		mFontSize = fontSize;
		return *this;
	}

	List& List::setColour(const glm::vec4 colour) {
		mColour = colour;
		return *this;
	}

	List& List::setBackgroundColour(const glm::vec4 colour) {
		mBackgroundColour = colour;
		return *this;
	}

	List& List::setPadding(const glm::vec4 padding) {
		mPadding = padding;
		return *this;
	}

	List& List::setItemPadding(const float padding) {
		mItemPadding = padding;
		return *this;
	}

	List& List::setSelectCallback(const std::optional<std::function<void(uint32_t)>>& callback) {
		mSelectCallback = callback;
		return *this;
	}

	void List::onHoverStop() {
		mHovered = UINT32_MAX;
	}

	void List::onPress(const glm::vec2 pos) {
		Component::onPress(pos);
		float itemY = mPadding.y - mItemPadding * 0.5f;
		render::UIRenderer& renderer = Genbliothinothon::getInstance()->getRenderContext().getUIRenderer();
		for (uint32_t item = 0; item < mItems.size(); item++) {
			const glm::vec2 size = renderer.getTextSize(mFontSize, mItems[item]);
			if (pos.y >= itemY && pos.y < itemY + size.y + mItemPadding) {
				mPressed = item;
				break;
			}
			itemY += size.y + mItemPadding;
		}
	}

	void List::onRelease() {
		Component::onRelease();
		mSelected = mPressed;
		mPressed = UINT32_MAX;
		if (mSelectCallback.has_value()) {
			mSelectCallback.value()(mSelected);
		}
	}

	void List::onMouseMove(const glm::vec2 pos) {
		float itemY = mPadding.y - mItemPadding * 0.5f;
		render::UIRenderer& renderer = Genbliothinothon::getInstance()->getRenderContext().getUIRenderer();
		for (uint32_t item = 0; item < mItems.size(); item++) {
			const glm::vec2 size = renderer.getTextSize(mFontSize, mItems[item]);
			if (pos.y >= itemY && pos.y < itemY + size.y + mItemPadding) {
				mHovered = item;
				break;
			}
			itemY += size.y + mItemPadding;
		}
	}

	void List::render(const glm::vec2 pos, render::UIRenderer& renderer) {
		const glm::vec2 size = calculateSize(renderer);
		renderer.rect(pos, size, mBackgroundColour);
		glm::vec2 itemPos = pos + glm::vec2{mPadding.x, mPadding.y};
		for (uint32_t item = 0; item < mItems.size(); item++) {
			std::string text = mItems[item];
			const glm::vec2 textSize = renderer.getTextSize(mFontSize, text);
			glm::vec4 backgroundColour = mBackgroundColour;
			if (mPressed == item) {
				backgroundColour *= 2.0f;
			}
			else if (mHovered == item) {
				backgroundColour *= 1.8f;
			}
			if (mSelected == item) {
				backgroundColour *= 1.5f;
			}
			renderer.rect({pos.x, itemPos.y - mItemPadding * 0.5f}, {size.x, textSize.y + mItemPadding}, backgroundColour);
			renderer.text(itemPos, mFontSize, text, mColour);
			itemPos.y += textSize.y + mItemPadding;
		}
	}

	glm::vec2 List::calculateSize(render::UIRenderer& renderer) {
		glm::vec2 textSize = {};
		for (const std::string& item: mItems) {
			glm::vec2 size = renderer.getTextSize(mFontSize, item);
			textSize.x = std::max(textSize.x, size.x);
			textSize.y += size.y;
		}
		return glm::vec2{mPadding.x + mPadding.z, mPadding.y + mPadding.w + (static_cast<float>(mItems.size()) - 1) * mItemPadding} + textSize;
	}
}
