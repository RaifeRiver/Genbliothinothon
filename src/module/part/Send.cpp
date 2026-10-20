#include "Send.h"

#include <algorithm>

#include "Background.h"
#include "Genbliothinothon.h"

namespace genbliothinothon::module::part {
	Send::Send(Module* parent, const glm::vec2 &pos, const glm::vec2& size, std::string name, const uint32_t channelCount): Part(parent, pos, size), mName(std::move(name)), mChannelCount(channelCount) {}

	void Send::setSendTarget(SendTarget* target) {
		mSendTarget = target;
	}

	void Send::setChannelCount(const uint32_t channelCount) {
		mChannelCount = channelCount;
	}

	void Send::click(glm::vec2) {
		mSelecting = !mSelecting;
		if (mSendTarget) {
			std::vector<Send*>& sends = mSendTarget->getInputs();
			sends.erase(std::ranges::find(sends, this));
			mSendTarget = nullptr;
		}
		setDirty(true);
	}

	void Send::clickOff(glm::vec2) {
		if (!mSelecting) {
			return;
		}
		Genbliothinothon* genbliothinothon = Genbliothinothon::getInstance();
		const glm::vec2 worldPos = genbliothinothon->getRenderContext().getMousePos() - genbliothinothon->getCameraPos();
		for (const std::unique_ptr<Module>& module: genbliothinothon->getModules()) {
			if (module.get() == getParent()) {
				continue;
			}
			const glm::vec2 modulePos = module->getPos();
			for (const std::unique_ptr<Part>& part: module->getParts()) {
				const glm::vec2 partPos = part->getPos() + modulePos;
				const glm::vec2 partSize = part->getSize();
				if (const auto sendTarget = dynamic_cast<SendTarget*>(part.get()); sendTarget && worldPos.x >= partPos.x && worldPos.x < partPos.x + partSize.x && worldPos.y >= partPos.y && worldPos.y < partPos.y + partSize.y) {
					if (!sendTarget->isMultiChannel() && mChannelCount > 1) {
						break;
					}
					mSendTarget = sendTarget;
					sendTarget->addInput(this);
					setDirty(true);
					break;
				}
			}
		}
		mSelecting = false;
	}

	void Send::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rect(pos, size, glm::vec4{0.06f, 0.06f, 0.11f, 1.0f});
		const float textHeight = size.y / 2.0f - 7.0f;
		std::string sendText = "Null (" + mName + ")";
		if (mSendTarget) {
			for (const std::unique_ptr<Part>& part: mSendTarget->getParent()->getParts()) {
				if (const Background* background = dynamic_cast<Background*>(part.get())) {
					sendText = background->getName() + "." + mSendTarget->getName();
					break;
				}
			}
		}
		else if (mSelecting) {
			sendText = "Select " + mName;
		}
		if (sendText.size() > 23) {
			sendText = sendText.substr(0, std::min<size_t>(sendText.size(), 20)) + "...";
		}
		renderer.text(pos + glm::vec2{2.0f, textHeight}, 14.0f, sendText, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
	}
}
