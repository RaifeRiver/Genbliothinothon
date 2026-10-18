#include "Genbliothinothon.h"

#include "module/Output.h"
#include "module/WaveGenerator.h"
#include "module/part/Background.h"

namespace genbliothinothon {
	Genbliothinothon::Genbliothinothon() {
		sInstance = this;

		mModules.emplace_back(std::make_unique<module::WaveGenerator>(glm::vec2{100.0f, 100.0f}));
		mModules.emplace_back(std::make_unique<module::WaveGenerator>(glm::vec2{500.0f, 200.0f}));
		mModules.emplace_back(std::make_unique<module::WaveGenerator>(glm::vec2{800.0f, 100.0f}));
		mModules.emplace_back(std::make_unique<module::Output>(glm::vec2{300.0f, 500.0f}));
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
			for (const std::unique_ptr<module::Module>& module: mModules) {
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
			}
			lastPressed = pressed;
			if (!moduleFound && pressed) {
				mCameraPos += mouseMovement;
			}

			if (dirty) {
				mAudioPlayer.rebuildShader(mModules);
			}

			mAudioPlayer.update(mRenderContext);

			uiRenderer.render(mRenderContext);
			mRenderContext.swapBuffers();
		}
	}
}
