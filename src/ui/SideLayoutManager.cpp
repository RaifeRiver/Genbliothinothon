#include "SideLayoutManager.h"

namespace genbliothinothon::ui {
	std::vector<glm::vec2> SideLayoutManager::getPositions(const glm::vec2 size, glm::vec2, std::vector<std::pair<std::unique_ptr<Component>, void*>>& components, render::UIRenderer& renderer) {
		std::vector<glm::vec2> positions;
		positions.reserve(components.size());
		for (auto& [component, data] : components) {
			glm::vec2 position;
			const glm::vec2 componentSize = component->calculateSize(renderer);
			switch (const SideLayoutData* sideData = static_cast<SideLayoutData*>(data); sideData ? sideData->side : Side::LEFT) {
				case Side::TOP_LEFT:
					position = {0.0f, 0.0f};
					break;
				case Side::LEFT:
					position = {0.0f, (size.y - componentSize.y) * 0.5f};
					break;
				case Side::BOTTOM_LEFT:
					position = {0.0f, size.y - componentSize.y};
					break;
				case Side::TOP_RIGHT:
					position = {size.x - componentSize.x, 0.0f};
					break;
				case Side::RIGHT:
					position = {size.x - componentSize.x, (size.y - componentSize.y) * 0.5f};
					break;
				case Side::BOTTOM_RIGHT:
					position = {size.x - componentSize.x, size.y - componentSize.y};
					break;
				case Side::TOP:
					position = {(size.x - componentSize.x) * 0.5f, 0.0f};
					break;
				case Side::BOTTOM:
					position = {(size.x - componentSize.x) * 0.5f, size.y - componentSize.y};
					break;
				case Side::CENTRE:
					position = {(size.x - componentSize.x) * 0.5f, (size.y - componentSize.y) * 0.5f};
					break;
				default:
					break;
			}
			positions.push_back(position);
		}
		return positions;
	}
}
