#include "Send.h"

#include "Background.h"
#include "Genbliothinothon.h"

namespace genbliothinothon::module::part {
	Send::Send(const glm::vec2 &pos, const glm::vec2& size): Part(pos, size) {}

	void Send::setSendTarget(Module* target) {
		mSendTarget = target;
	}

	void Send::click(glm::vec2) {
		mSelecting = !mSelecting;
		mSendTarget = nullptr;
	}

	void Send::clickOff(glm::vec2) {
		if (!mSelecting) {
			return;
		}
		Genbliothinothon* genbliothinothon = Genbliothinothon::getInstance();
		const glm::vec2 worldPos = genbliothinothon->getRenderContext().getMousePos() - genbliothinothon->getCameraPos();
		for (const std::unique_ptr<Module>& module: genbliothinothon->getModules()) {
			const glm::vec2 modulePos = module->getPos();
			if (const glm::vec2 moduleSize = module->getSize(); module->hasInput() && worldPos.x >= modulePos.x && worldPos.x <= modulePos.x + moduleSize.x && worldPos.y >= modulePos.y && worldPos.y <= modulePos.y + moduleSize.y) {
				mSendTarget = module.get();
				break;
			}
		}
		mSelecting = false;
	}

	void Send::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rect(pos, size, glm::vec4{0.06f, 0.06f, 0.11f, 1.0f});
		const float textHeight = size.y / 2.0f - 7.0f;
		std::string sendText = "Null send";
		if (mSendTarget) {
			for (const std::unique_ptr<Part>& part: mSendTarget->getParts()) {
				if (const Background* background = dynamic_cast<Background*>(part.get())) {
					sendText = background->getName();
					break;
				}
			}
		}
		else if (mSelecting) {
			sendText = "Click to select...";
		}
		renderer.text(pos + glm::vec2{2.0f, textHeight}, 14.0f, sendText, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
	}
}
