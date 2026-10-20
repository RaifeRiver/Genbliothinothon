#include "Genbliothinothon.h"

#include <fstream>

#include "module/CSVSequencer.h"
#include "module/MidiSequencer.h"
#include "module/NoiseSource.h"
#include "module/Output.h"
#include "module/Oscillator.h"
#include "module/part/Background.h"
#include "module/part/RemoveButton.h"
#include "module/part/Send.h"

namespace genbliothinothon {
	Genbliothinothon::Genbliothinothon() {
		sInstance = this;

		mModules.emplace_back(std::make_unique<module::CSVSequencer>(glm::vec2{100.0f, 100.0f}));
		dynamic_cast<module::CSVSequencer*>(mModules.back().get())->getTextInput()->setText("../res/songs/song_1_voice1.csv");

		mModules.emplace_back(std::make_unique<module::CSVSequencer>(glm::vec2{100.0f, 250.0f}));
		dynamic_cast<module::CSVSequencer*>(mModules.back().get())->getTextInput()->setText("../res/songs/song_1_voice2.csv");

		mModules.emplace_back(std::make_unique<module::Oscillator>(glm::vec2{310.0f, 100.0f}));
		mModules.emplace_back(std::make_unique<module::Oscillator>(glm::vec2{310.0f, 250.0f}));
		mModules.emplace_back(std::make_unique<module::Output>(glm::vec2{520.0f, 100.0f}));

		mModuleTypes.emplace_back("CSV Sequencer", [](const glm::vec2 pos) {
			return std::make_unique<module::CSVSequencer>(pos);
		});
		mModuleTypes.emplace_back("MIDI Sequencer", [](const glm::vec2 pos) {
			return std::make_unique<module::MidiSequencer>(pos);
		});
		mModuleTypes.emplace_back("Noise Source", [](const glm::vec2 pos) {
			return std::make_unique<module::NoiseSource>(pos);
		});
		mModuleTypes.emplace_back("Oscillator", [](const glm::vec2 pos) {
			return std::make_unique<module::Oscillator>(pos);
		});
	}

	void Genbliothinothon::run() {
		bool lastPressed = false;

		while (!mRenderContext.shouldClose()) {
			mRenderContext.clearColour(0.05f, 0.05f, 0.1f);

			render::UIRenderer& uiRenderer = mRenderContext.getUIRenderer();

			const glm::vec2 mousePos = mRenderContext.getMousePos();
			const glm::vec2 mouseMovement = mRenderContext.getMouseMovement();
			const bool pressed = mRenderContext.isButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
			bool moduleFound = false;
			bool dragging = false;
			bool dirty = false;
			for (size_t i = 0; i < mModules.size(); i++) {
				const std::unique_ptr<module::Module>& module = mModules[i];
				glm::vec2 modulePos = module->getPos() + mCameraPos;
				if (!pressed) {
					module->setDragged(false);
					module->endDrag();
				}
				const glm::vec2 moduleSize = module->getSize();
				const bool moduleHovered = mousePos.x >= modulePos.x && mousePos.x <= modulePos.x + moduleSize.x && mousePos.y >= modulePos.y && mousePos.y <= modulePos.y + moduleSize.y;
				const bool dragged = module->isDragged();
				moduleFound |= moduleHovered || dragged;
				if (!dragged && pressed && !lastPressed) {
					for (const std::unique_ptr<module::part::Part>& part : module->getParts()) {
						const glm::vec2 partPos = part->getPos() + modulePos;
						if (const glm::vec2 partSize = part->getSize(); mousePos.x >= partPos.x && mousePos.x <= partPos.x + partSize.x && mousePos.y >= partPos.y && mousePos.y <= partPos.y + partSize.y) {
							if (dynamic_cast<module::part::RemoveButton*>(part.get())) {
								for (const std::unique_ptr<module::Module>& m : mModules) {
									if (m.get() == module.get()) {
										continue;
									}
									for (const std::unique_ptr<module::part::Part>& p : m->getParts()) {
										for (const std::unique_ptr<module::part::Part>& p2 : module->getParts()) {
											if (const auto send = dynamic_cast<module::part::Send*>(p.get()); send && send->getSendTarget() == p2.get()) {
												send->setSendTarget(nullptr);
											}
										}
									}
								}
								mModules.erase(mModules.begin() + static_cast<uint32_t>(i--));
								dirty = true;
								goto end_module_loop;
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
				module->render(mCameraPos, mRenderContext);
				if (module->isDirty()) {
					module->clearDirty();
					dirty = true;
				}
				end_module_loop:;
			}
			if (!moduleFound && pressed) {
				mCameraPos += mouseMovement;
			}

			glm::vec2 windowSize = mRenderContext.getWindowSize();
			uiRenderer.rect({0, 0}, {150, windowSize.y}, {0.08f, 0.08f, 0.14f, 1.0f});
			for (size_t i = 0; i < mModuleTypes.size(); i++) {
				std::string& name = mModuleTypes[i].first;
				uiRenderer.text({4, 4 + i * 20}, 16.0f, name, {1.0f, 1.0f, 1.0f, 1.0f});
				if (mousePos.x >= 4 && mousePos.x <= 150 && static_cast<size_t>(mousePos.y) >= 4 + i * 20 && static_cast<size_t>(mousePos.y) <= 20 + i * 20 && (pressed && !lastPressed)) {
					mModules.emplace_back(mModuleTypes[i].second(glm::vec2{200.0f, 200.0f} - mCameraPos));
				}
			}

			if (dirty) {
				mAudioPlayer.rebuildShader(mModules);
			}

			mAudioPlayer.update(mRenderContext);

			uiRenderer.render(mRenderContext);

			lastPressed = pressed;

			mRenderContext.swapBuffers();
		}
	}
}
