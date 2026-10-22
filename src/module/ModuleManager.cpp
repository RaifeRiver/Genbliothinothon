#include "ModuleManager.h"

#include <ranges>

#include "Genbliothinothon.h"
#include "Output.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	ModuleManager::ModuleManager() {
		part::Part::initParts();

		mModules[mNextID++] = Module::createModule("Output", glm::vec2{500.0f, 100.0f});
	}

	void ModuleManager::addModule(std::unique_ptr<Module> module) {
		module->setID(mNextID);
		mModules[mNextID++] = std::move(module);
	}

	Module* ModuleManager::getModule(const uint32_t id) {
		return mModules[id].get();
	}

	void ModuleManager::removeModule(const uint32_t id) {
		mModules.erase(id);
	}

	void ModuleManager::update(Genbliothinothon& genbliothinothon) {
		static bool lastPressed = false;

		render::RenderContext& renderContext = genbliothinothon.getRenderContext();
		render::UIRenderer& uiRenderer = renderContext.getUIRenderer();
		glm::vec2& cameraPos = genbliothinothon.getCameraPos();

		const glm::vec2 mousePos = renderContext.getMousePos();
		const glm::vec2 mouseMovement = renderContext.getMouseMovement();
		const bool pressed = renderContext.isButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
		bool moduleFound = false;
		bool dragging = false;
		bool dirty = false;
		std::vector<uint32_t> modulesToRemove;
		for (const auto& [id, module] : mModules) {
			glm::vec2 modulePos = module->getPos() + cameraPos;
			if (!pressed) {
				module->setDragged(false);
				module->endDrag();
			}
			const glm::vec2 moduleSize = module->getSize();
			const bool moduleHovered = mousePos.x >= modulePos.x && mousePos.x <= modulePos.x + moduleSize.x && mousePos.y >= modulePos.y && mousePos.y <= modulePos.y + moduleSize.y;
			const bool dragged = module->isDragged();
			moduleFound |= moduleHovered || dragged;
			if (!dragged && pressed && !lastPressed) {
				for (const std::unique_ptr<part::Part>& part : module->getParts()) {
					const glm::vec2 partPos = part->getPos() + modulePos;
					if (const glm::vec2 partSize = part->getSize(); mousePos.x >= partPos.x && mousePos.x <= partPos.x + partSize.x && mousePos.y >= partPos.y && mousePos.y <= partPos.y + partSize.y) {
						if (dynamic_cast<part::RemoveButton*>(part.get())) {
							for (const std::unique_ptr<Module>& m : mModules | std::views::values) {
								if (m.get() == module.get()) {
									continue;
								}
								for (const std::unique_ptr<part::Part>& p : m->getParts()) {
									for (const std::unique_ptr<part::Part>& p2 : module->getParts()) {
										if (const auto send = dynamic_cast<part::Send*>(p.get()); send && send->getSendTarget() == p2.get()) {
											send->setSendTarget(nullptr);
										}
									}
								}
							}
							modulesToRemove.push_back(id);
							dirty = true;
						}
						part->click(mousePos - partPos);
					}
					else {
						part->clickOff(mousePos - partPos);
					}
				}
			}
			if (dragged || moduleHovered && pressed && !dragging) {
				module->setDragged(true);
				module->drag(mouseMovement);
				dragging = true;
			}
			module->render(cameraPos, renderContext);
			if (module->isDirty()) {
				module->clearDirty();
				dirty = true;
			}
		}
		for (uint32_t id : modulesToRemove) {
			mModules.erase(id);
		}
		modulesToRemove.clear();
		if (!moduleFound && pressed) {
			cameraPos += mouseMovement;
		}

		glm::vec2 windowSize = renderContext.getWindowSize();
		renderContext.getUIRenderer().rect({0, 0}, {150, windowSize.y}, {0.08f, 0.08f, 0.14f, 1.0f});
		std::vector<std::tuple<std::string, std::function<std::unique_ptr<Module>(glm::vec2)>, std::function<std::unique_ptr<Module>(uint32_t, util::FileIStream&)>>>& moduleTypes = Module::getModules();
		for (size_t i = 0; i < moduleTypes.size(); i++) {
			auto& [name, constructor, _] = moduleTypes[i];
			uiRenderer.text({4, 4 + i * 20}, 16.0f, name, {1.0f, 1.0f, 1.0f, 1.0f});
			if (mousePos.x >= 4 && mousePos.x <= 150 && static_cast<size_t>(mousePos.y) >= 4 + i * 20 && static_cast<size_t>(mousePos.y) <= 20 + i * 20 && pressed && !lastPressed) {
				std::unique_ptr<Module> module = constructor(glm::vec2{200.0f, 200.0f} - cameraPos);
				module->setID(mNextID);
				mModules[mNextID++] = std::move(module);
			}
		}

		if (dirty) {
			genbliothinothon.getAudioPlayer().rebuildShader(*this);
		}

		lastPressed = pressed;
	}

	void ModuleManager::save(util::FileOStream& stream) const {
		stream << mNextID;
		stream << static_cast<uint32_t>(mModules.size());
		for (const std::unique_ptr<Module>& module: mModules | std::views::values) {
			module->save(stream);
		}
	}

	void ModuleManager::load(const uint32_t version, util::FileIStream& stream) {
		mModules.clear();
		stream >> mNextID;
		uint32_t moduleCount;
		stream >> moduleCount;
		mModules.reserve(moduleCount);
		for (uint32_t i = 0; i < moduleCount; i++) {
			std::unique_ptr<Module> module = Module::loadModule(version, stream);
			mModules[module->getID()] = std::move(module);
		}
	}
}
