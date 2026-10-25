#pragma once

#include "Component.h"

namespace genbliothinothon::ui {
	class LayoutManager {
	public:
		virtual std::vector<glm::vec2> getPositions(glm::vec2 size, glm::vec2 componentPadding, std::vector<std::pair<std::unique_ptr<Component>, void*>>& components, render::UIRenderer& renderer) = 0;

		virtual ~LayoutManager() = default;
	};
}
