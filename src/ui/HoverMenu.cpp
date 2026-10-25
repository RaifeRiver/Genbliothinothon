#include "HoverMenu.h"

#include <utility>

#include "Genbliothinothon.h"

namespace genbliothinothon::ui {
	HoverMenu::HoverMenu(std::string label) : mLabel(std::move(label)) {}

	HoverMenu& HoverMenu::setLabel(const std::string& label) {
		mLabel = label;
		return *this;
	}

	HoverMenu& HoverMenu::setFontSize(const float fontSize) {
		mFontSize = fontSize;
		return *this;
	}

	HoverMenu& HoverMenu::setColour(const glm::vec4 colour) {
		mColour = colour;
		return *this;
	}

	HoverMenu& HoverMenu::setBackgroundColour(const glm::vec4 colour) {
		mBackgroundColour = colour;
		return *this;
	}

	HoverMenu& HoverMenu::setPadding(const glm::vec4 padding) {
		mPadding = padding;
		return *this;
	}

	HoverMenu& HoverMenu::setMenu(std::unique_ptr<Component> menu) {
		mMenu = std::move(menu);
		return *this;
	}

	void HoverMenu::onHover() {
		Component::onHover();
		mOpen = true;
	}

	void HoverMenu::onPress(const glm::vec2 pos) {
		Component::onPress(pos);
		if (mOpen && mMenu) {
			if (const glm::vec2 size = calculateSize(Genbliothinothon::getInstance()->getRenderContext().getUIRenderer()); pos.y > size.y) {
				mMenu->onPress(pos - glm::vec2{0.0f, size.y});
			}
		}
	}

	void HoverMenu::onRelease() {
		Component::onRelease();
		if (mOpen && mMenu && mMenu->isPressed()) {
			mMenu->onRelease();
		}
	}

	void HoverMenu::onMouseMove(const glm::vec2 pos) {
		if (mMenu) {
			if (const glm::vec2 size = calculateSize(Genbliothinothon::getInstance()->getRenderContext().getUIRenderer()); pos.y > size.y && mOpen) {
				mMenu->setHovered(true);
				mMenu->onMouseMove(pos - glm::vec2{0.0f, size.y});
			}
			else if (mMenu->isHovered()) {
				mMenu->setHovered(false);
			}
		}
	}

	void HoverMenu::render(const glm::vec2 pos, render::UIRenderer& renderer) {
		const glm::vec4 backgroundColour = mOpen || isHovered() ? mBackgroundColour * 1.5f : mBackgroundColour;
		const glm::vec2 size = calculateSize(renderer);
		renderer.rect(pos, size, backgroundColour);
		renderer.text(pos + glm::vec2{mPadding.x, mPadding.y}, mFontSize, mLabel, mColour);
		if (mOpen) {
			if (mMenu) {
				mMenu->render(pos + glm::vec2{0.0f, size.y}, renderer);
				const glm::vec2 menuSize = mMenu->calculateSize(renderer);
				if (const glm::vec2 localMousePos = Genbliothinothon::getInstance()->getRenderContext().getMousePos() - pos; localMousePos.x < 0.0f || localMousePos.y < 0.0f || (localMousePos.y < size.y ? localMousePos.x > size.x : localMousePos.y > size.y + menuSize.y || localMousePos.x > menuSize.x)) {
					mOpen = false;
				}
			}
			else {
				mOpen = false;
			}
		}
	}

	glm::vec2 HoverMenu::calculateSize(render::UIRenderer& renderer) {
		return renderer.getTextSize(mFontSize, mLabel) + glm::vec2{mPadding.x + mPadding.z, mPadding.y + mPadding.w};
	}

	glm::vec4 HoverMenu::calculateInteractionSize(render::UIRenderer& renderer) {
		glm::vec2 size = calculateSize(renderer);
		if (mOpen && mMenu) {
			const glm::vec4 menuSize = mMenu->calculateInteractionSize(renderer);
			return {0.0f, 0.0f, std::max(size.x, menuSize.z), std::max(size.y, menuSize.w + size.y)};
		}
		return {0.0f, 0.0f, size};
	}
}
