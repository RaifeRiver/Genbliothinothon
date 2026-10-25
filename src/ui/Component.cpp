#include "Component.h"

namespace genbliothinothon::ui {
	void Component::setHovered(const bool hovered) {
		if (!mHovered && hovered) {
			onHover();
		}
		else if (mHovered && !hovered) {
			onHoverStop();
		}
		mHovered = hovered;
	}

	void Component::onHover() {}

	void Component::onHoverStop() {}

	void Component::onPress(glm::vec2) {
		mPressed = true;
	}

	void Component::onRelease() {
		mPressed = false;
	}

	void Component::onMouseMove(glm::vec2) {}

	void Component::remove() {
		mRemove = true;
	}

	glm::vec4 Component::calculateInteractionSize(render::UIRenderer& renderer) {
		return {0.0f, 0.0f, calculateSize(renderer)};
	}
}
