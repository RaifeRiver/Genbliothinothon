#include "ModuleArea.h"

#include <ranges>

#include "Genbliothinothon.h"
#include "module/Module.h"
#include "module/ModuleManager.h"
#include "module/part/RemoveButton.h"

namespace genbliothinothon::ui {
	ModuleArea::ModuleArea(const glm::vec2 size) : mSize(size) {}

	void ModuleArea::setSize(const glm::vec2 size) {
		mSize = size;
	}

	void ModuleArea::onPress(const glm::vec2 p) {
		Component::onPress(p);
		Genbliothinothon* genbliothinothon = Genbliothinothon::getInstance();
		const glm::vec2 pos = genbliothinothon->getRenderContext().getMousePos();
		const glm::vec2 cameraPos = genbliothinothon->getCameraPos();
		std::vector<uint32_t> modulesToRemove;
		module::ModuleManager& moduleManager = genbliothinothon->getModuleManager();
		for (std::unordered_map<uint32_t, std::unique_ptr<module::Module>>& modules = moduleManager.getModules(); const auto& [id, module]: modules) {
			const glm::vec2 modulePos = module->getPos() + cameraPos;
			const std::vector<std::unique_ptr<module::part::Part>>& parts = module->getParts();
			if (const glm::vec2 moduleSize = module->getSize(); pos.x >= modulePos.x && pos.x <= modulePos.x + moduleSize.x && pos.y >= modulePos.y && pos.y <= modulePos.y + moduleSize.y) {
				for (const std::unique_ptr<module::part::Part>& part : parts) {
					const glm::vec2 partPos = part->getPos() + modulePos;
					if (const glm::vec2 partSize = part->getSize(); pos.x >= partPos.x && pos.x <= partPos.x + partSize.x && pos.y >= partPos.y && pos.y <= partPos.y + partSize.y) {
						if (dynamic_cast<module::part::RemoveButton*>(part.get())) {
							modulesToRemove.push_back(id);
						}
						part->click(pos - partPos);
					}
					else {
						part->clickOff(pos - partPos, pos);
					}
				}
				module->setDragged(true);
			}
			else {
				for (const std::unique_ptr<module::part::Part>& part : parts) {
					const glm::vec2 partPos = part->getPos() + modulePos;
					part->clickOff(pos - partPos, pos - cameraPos);
				}
			}
		}
		for (const uint32_t id : modulesToRemove) {
			moduleManager.removeModule(id);
		}
	}

	void ModuleArea::onRelease() {
		Component::onRelease();
		Genbliothinothon* genbliothinothon = Genbliothinothon::getInstance();
		for (std::unordered_map<uint32_t, std::unique_ptr<module::Module>>& modules = genbliothinothon->getModuleManager().getModules(); const std::unique_ptr<module::Module>& module: modules | std::views::values) {
			if (module->isDragged()) {
				module->setDragged(false);
				module->endDrag();
			}
		}
	}

	void ModuleArea::onMouseMove(const glm::vec2) {
		Genbliothinothon* genbliothinothon = Genbliothinothon::getInstance();
		const glm::vec2 pos = genbliothinothon->getRenderContext().getMousePos();
		static glm::vec2 lastPos = pos;
		const glm::vec2 movement = pos - lastPos;
		lastPos = pos;
		if (isPressed()) {
			const glm::vec2 cameraPos = genbliothinothon->getCameraPos();
			for (std::unordered_map<uint32_t, std::unique_ptr<module::Module>>& modules = genbliothinothon->getModuleManager().getModules(); const std::unique_ptr<module::Module>& module: modules | std::views::values) {
				if (module->isDragged()) {
					module->drag(movement, pos - cameraPos);
					return;
				}
			}
			genbliothinothon->getCameraPos() += movement;
		}
	}

	void ModuleArea::render(const glm::vec2 pos, render::UIRenderer& renderer) {
		renderer.rect(pos, calculateSize(renderer), {0.05f, 0.05f, 0.1f, 1.0f});
		Genbliothinothon* genbliothinothon = Genbliothinothon::getInstance();
		module::ModuleManager& moduleManager = genbliothinothon->getModuleManager();
		render::RenderContext& renderContext = genbliothinothon->getRenderContext();
		std::unordered_map<uint32_t, std::unique_ptr<module::Module>>& modules = moduleManager.getModules();
		const glm::vec2 cameraPos = genbliothinothon->getCameraPos();
		for (const std::unique_ptr<module::Module>& module: modules | std::views::values) {
			module->render(cameraPos, renderContext);
		}
	}

	glm::vec2 ModuleArea::calculateSize(render::UIRenderer&) {
		glm::vec2 windowSize = Genbliothinothon::getInstance()->getRenderContext().getWindowSize();
		return {mSize.x > 0.0f ? mSize.x : windowSize.x, mSize.y > 0.0f ? mSize.y : windowSize.y};
	}
}
