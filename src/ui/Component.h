#pragma once

#include "glm/vec2.hpp"

#include "render/UIRenderer.h"

namespace genbliothinothon::ui {
	class Component {
	public:
		[[nodiscard]] bool isHovered() const {
			return mHovered;
		}

		void setHovered(bool hovered);

		[[nodiscard]] bool isPressed() const {
			return mPressed;
		}

		virtual void onHover();

		virtual void onHoverStop();

		virtual void onPress(glm::vec2 pos);

		virtual void onRelease();

		virtual void onMouseMove(glm::vec2 pos);

		[[nodiscard]] bool shouldRemove() const {
			return mRemove;
		}

		void remove();

		virtual void render(glm::vec2 pos, render::UIRenderer& renderer) = 0;

		[[nodiscard]] virtual glm::vec2 calculateSize(render::UIRenderer& renderer) = 0;

		[[nodiscard]] virtual glm::vec4 calculateInteractionSize(render::UIRenderer& renderer);

		virtual ~Component() = default;

	private:
		bool mHovered = false;
		bool mPressed = false;
		bool mRemove = false;
	};
}
