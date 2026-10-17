#include "Genbliothinothon.h"

#include "module/WaveGenerator.h"

namespace genbliothinothon {
	Genbliothinothon::Genbliothinothon() {
		mModules.emplace_back(std::make_unique<module::WaveGenerator>(glm::vec2{100.0f, 100.0f}));
	}

	void Genbliothinothon::run() {
		while (!mRenderContext.shouldClose()) {
			mRenderContext.clearColour(0.05f, 0.05f, 0.1f);

			render::UIRenderer& uiRenderer = mRenderContext.getUIRenderer();

			const glm::vec2 mousePos = mRenderContext.getMousePos();
			bool pressed = mRenderContext.isButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
			for (const std::unique_ptr<module::Module>& module: mModules) {
				glm::vec2 modulePos = module->getPos();
				if (!pressed) {
					module->setDragged(false);
				}
				if (const glm::vec2 moduleSize = module->getSize(); module->isDragged() || mousePos.x >= modulePos.x && mousePos.x <= modulePos.x + moduleSize.x && mousePos.y >= modulePos.y && mousePos.y <= modulePos.y + moduleSize.y && pressed) {
					module->setDragged(true);
					module->setPos(modulePos + mRenderContext.getMouseMovement());
				}
				module->render({0.0f, 0.0f}, mRenderContext);
			}

			mAudioPlayer.update(mRenderContext);

			uiRenderer.render(mRenderContext);
			mRenderContext.swapBuffers();
		}
	}
}
