#pragma once

#include "LayoutManager.h"

namespace genbliothinothon::ui {
	enum class Direction {
		HORIZONTAL,
		VERTICAL
	};

	class SimpleLayoutManager : public LayoutManager {
	public:
		explicit SimpleLayoutManager(Direction direction);

		std::vector<glm::vec2> getPositions(glm::vec2 size, glm::vec2 componentPadding, std::vector<std::pair<std::unique_ptr<Component>, void*>>& components, render::UIRenderer& renderer) override;

	private:
		Direction mDirection;
	};
}
