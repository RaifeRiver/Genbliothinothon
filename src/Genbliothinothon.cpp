#include "Genbliothinothon.h"

#include "module/WaveGenerator.h"

namespace genbliothinothon {
	Genbliothinothon::Genbliothinothon() {
		mModules.emplace_back(std::make_unique<module::WaveGenerator>(glm::vec2{100.0f, 100.0f}));
		mModules.emplace_back(std::make_unique<module::WaveGenerator>(glm::vec2{500.0f, 200.0f}));
		mModules.emplace_back(std::make_unique<module::WaveGenerator>(glm::vec2{800.0f, 100.0f}));
		mModules.emplace_back(std::make_unique<module::WaveGenerator>(glm::vec2{300.0f, 500.0f}));
	}

	void Genbliothinothon::run() {
		bool lastPressed = false;

		while (!mRenderContext.shouldClose()) {
			mRenderContext.clearColour(0.05f, 0.05f, 0.1f);

			render::UIRenderer& uiRenderer = mRenderContext.getUIRenderer();

			const glm::vec2 mousePos = mRenderContext.getMousePos();
			const bool pressed = mRenderContext.isButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
			for (const std::unique_ptr<module::Module>& module: mModules) {
				glm::vec2 modulePos = module->getPos();
				if (!pressed) {
					module->setDragged(false);
				}
				if (const glm::vec2 moduleSize = module->getSize(); module->isDragged() || mousePos.x >= modulePos.x && mousePos.x <= modulePos.x + moduleSize.x && mousePos.y >= modulePos.y && mousePos.y <= modulePos.y + moduleSize.y && pressed) {
					module->setDragged(true);
					module->setPos(modulePos + mRenderContext.getMouseMovement());
				}
				if (pressed && !lastPressed) {
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
				module->render({0.0f, 0.0f}, mRenderContext);
			}
			lastPressed = pressed;

			mAudioPlayer.update(mRenderContext);

			uiRenderer.render(mRenderContext);
			mRenderContext.swapBuffers();
		}
	}
}
