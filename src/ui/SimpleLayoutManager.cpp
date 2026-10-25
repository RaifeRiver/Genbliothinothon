#include "SimpleLayoutManager.h"

#include <ranges>

namespace genbliothinothon::ui {
	SimpleLayoutManager::SimpleLayoutManager(const Direction direction) : mDirection(direction) {}

	std::vector<glm::vec2> SimpleLayoutManager::getPositions(glm::vec2, const glm::vec2 componentPadding, std::vector<std::pair<std::unique_ptr<Component>, void*>>& components, render::UIRenderer& renderer) {
		std::vector<glm::vec2> positions;
		positions.reserve(components.size());
		glm::vec2 nextPosition = {0.0f, 0.0f};
		for (const std::unique_ptr<Component>& component: components | std::views::keys) {
			const glm::vec2 componentSize = component->calculateSize(renderer);
			positions.push_back(nextPosition);
			switch (mDirection) {
				case Direction::HORIZONTAL:
					nextPosition.x += componentSize.x + componentPadding.x;
					break;
				case Direction::VERTICAL:
					nextPosition.y += componentSize.y + componentPadding.y;
					break;
				default:
					break;
			}
		}
		return positions;
	}
}
