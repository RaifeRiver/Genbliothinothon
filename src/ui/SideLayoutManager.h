#pragma once

#include "LayoutManager.h"

namespace genbliothinothon::ui {
	enum class Side {
		TOP_LEFT,
		LEFT,
		BOTTOM_LEFT,
		TOP_RIGHT,
		RIGHT,
		BOTTOM_RIGHT,
		TOP,
		BOTTOM,
		CENTRE
	};

	struct SideLayoutData {
		Side side;
	};

	class SideLayoutManager : public LayoutManager {
	public:
		std::vector<glm::vec2> getPositions(glm::vec2 size, glm::vec2 componentPadding, std::vector<std::pair<std::unique_ptr<Component>, void*>>& components, render::UIRenderer& renderer) override;
	};
}
