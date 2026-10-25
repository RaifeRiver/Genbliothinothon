#include "Container.h"

#include <ranges>

#include "Genbliothinothon.h"

namespace genbliothinothon::ui {
	Container::Container(const glm::vec2 size): mSize(size) {}

	Container& Container::setLayoutManager(std::unique_ptr<LayoutManager> layoutManager) {
		mLayoutManager = std::move(layoutManager);
		return *this;
	}

	Container& Container::setBackgroundColour(const glm::vec4 backgroundColour) {
		mBackgroundColour = backgroundColour;
		return *this;
	}

	Container& Container::setPadding(const glm::vec4 padding) {
		mPadding = padding;
		return *this;
	}

	Container& Container::setComponentPadding(const glm::vec2 componentPadding) {
		mComponentPadding = componentPadding;
		return *this;
	}

	void Container::setSize(const glm::vec2 size) {
		mSize = size;
	}

	void Container::onHoverStop() {
		for (uint32_t i = 0; i < mChildren.size(); i++) {
			const std::unique_ptr<Component>& child = mChildren[i].first;
			child->setHovered(false);
			if (child->shouldRemove()) {
				mChildren.erase(mChildren.begin() + i);
				i--;
			}
		}
	}

	void Container::onPress(const glm::vec2 pos) {
		Component::onPress(pos);
		render::UIRenderer& renderer = Genbliothinothon::getInstance()->getRenderContext().getUIRenderer();
		const std::vector<glm::vec2> childPositions = mLayoutManager->getPositions(calculateSize(renderer) - glm::vec2{mPadding.x + mPadding.z, mPadding.y + mPadding.w}, mComponentPadding, mChildren, renderer);
		for (uint32_t i = 0; i < mChildren.size(); i++) {
			Component* child = mChildren[i].first.get();
			const glm::vec2 childPos = pos - childPositions[i] - glm::vec2{mPadding.x, mPadding.y};
			if (const glm::vec4 childSize = child->calculateInteractionSize(renderer); childPos.x >= childSize.x && childPos.x <= childSize.z && childPos.y >= childSize.y && childPos.y <= childSize.w) {
				child->onPress(childPos);
				if (child->shouldRemove()) {
					mChildren.erase(mChildren.begin() + i);
					i--;
				}
			}
		}
	}

	void Container::onRelease() {
		Component::onRelease();
		for (uint32_t i = 0; i < mChildren.size(); i++) {
			if (Component* child = mChildren[i].first.get(); child->isPressed()) {
				child->onRelease();
				if (child->shouldRemove()) {
					mChildren.erase(mChildren.begin() + i);
					i--;
				}
			}
		}
	}

	void Container::onMouseMove(const glm::vec2 pos) {
		render::UIRenderer& renderer = Genbliothinothon::getInstance()->getRenderContext().getUIRenderer();
		const std::vector<glm::vec2> childPositions = mLayoutManager->getPositions(calculateSize(renderer) - glm::vec2{mPadding.x + mPadding.z, mPadding.y + mPadding.w}, mComponentPadding, mChildren, renderer);
		for (uint32_t i = 0; i < mChildren.size(); i++) {
			Component* child = mChildren[i].first.get();
			const glm::vec2 childPos = pos - childPositions[i] - glm::vec2{mPadding.x, mPadding.y};
			if (const glm::vec4 childSize = child->calculateInteractionSize(renderer); childPos.x >= childSize.x && childPos.x <= childSize.z && childPos.y >= childSize.y && childPos.y <= childSize.w) {
				child->setHovered(true);
				child->onMouseMove(childPos);
			}
			else if (child->isHovered()) {
				child->setHovered(false);
			}
			if (child->shouldRemove()) {
				mChildren.erase(mChildren.begin() + i);
				i--;
			}
		}
	}

	void Container::render(const glm::vec2 pos, render::UIRenderer& renderer) {
		const glm::vec2 size = calculateSize(renderer);
		renderer.rect(pos, size, mBackgroundColour);
		const glm::vec2 childPos = pos + glm::vec2{mPadding.x, mPadding.y};
		const std::vector<glm::vec2> childPositions = mLayoutManager->getPositions(size - glm::vec2{mPadding.x + mPadding.z, mPadding.y + mPadding.w}, mComponentPadding, mChildren, renderer);
		for (uint32_t i = 0; i < mChildren.size(); i++) {
			Component* child = mChildren[i].first.get();
			child->render(childPos + childPositions[i], renderer);
			if (child->shouldRemove()) {
				mChildren.erase(mChildren.begin() + i);
				i--;
			}
		}
	}

	glm::vec2 Container::calculateSize(render::UIRenderer&) {
		glm::vec2 windowSize = Genbliothinothon::getInstance()->getRenderContext().getWindowSize();
		return {mSize.x > 0.0f ? mSize.x : windowSize.x, mSize.y > 0.0f ? mSize.y : windowSize.y};
	}

	glm::vec4 Container::calculateInteractionSize(render::UIRenderer& renderer) {
		glm::vec4 interactionSize = {0.0f, 0.0f, 0.0f, 0.0f};
		const std::vector<glm::vec2> childPositions = mLayoutManager->getPositions(calculateSize(renderer) - glm::vec2{mPadding.x + mPadding.z, mPadding.y + mPadding.w}, mComponentPadding, mChildren, renderer);
		for (uint32_t i = 0; i < mChildren.size(); i++) {
			const std::unique_ptr<Component>& child = mChildren[i].first;
			const glm::vec2 childPos = childPositions[i];
			const glm::vec4 childSize = child->calculateInteractionSize(renderer);
			interactionSize = {std::min(interactionSize.x, childPos.x + childSize.x), std::min(interactionSize.y, childPos.y + childSize.y), std::max(interactionSize.z, childPos.x + childSize.z), std::max(interactionSize.w, childPos.y + childSize.w)};
		}
		return interactionSize;
	}
}
