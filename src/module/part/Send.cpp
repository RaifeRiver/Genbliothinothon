#include "Send.h"

#include <algorithm>
#include <ranges>

#include "Background.h"
#include "Genbliothinothon.h"
#include "SendTarget.h"

namespace genbliothinothon::module::part {
	Send::Send(Module* parent, const glm::vec2 &pos, const glm::vec2& size, std::string name, const uint32_t channelCount): Part(sType, parent, pos, size), mName(std::move(name)), mChannelCount(channelCount) {}

	Send::Send(const uint32_t version, util::FileIStream& stream) : Part(version, stream) {
		stream >> mName;
		stream >> mChannelCount;
		bool hasSendTarget;
		stream >> hasSendTarget;
		if (hasSendTarget) {
			mSendTarget = {.module = 0, .part = 0};
			stream >> mSendTarget->module;
			stream >> mSendTarget->part;
		}
	}

	void Send::setSendTarget(const std::optional<PartID> target) {
		mSendTarget = target;
	}

	void Send::setChannelCount(const uint32_t channelCount) {
		mChannelCount = channelCount;
	}

	void Send::click(glm::vec2) {
		mSelecting = !mSelecting;
		if (mSendTarget) {
			std::vector<PartID>& sends = reinterpret_cast<SendTarget*>(Genbliothinothon::getInstance()->getModuleManager().getModule(mSendTarget->module)->getParts()[mSendTarget->part].get())->getInputs();
			Module* parent = getParent();
			uint32_t index = 0;
			const std::vector<std::unique_ptr<Part>>& parts = parent->getParts();
			for (uint32_t i = 0; i < parts.size(); i++) {
				if (parts[i].get() == this) {
					index = i;
					break;
				}
			}
			sends.erase(std::ranges::find(sends, PartID{.module = parent->getID(), .part = index}));
			mSendTarget = {};
		}
		setDirty(true);
	}

	void Send::clickOff(glm::vec2) {
		if (!mSelecting) {
			return;
		}
		Genbliothinothon* genbliothinothon = Genbliothinothon::getInstance();
		const glm::vec2 worldPos = genbliothinothon->getRenderContext().getMousePos() - genbliothinothon->getCameraPos();
		for (const std::unique_ptr<Module>& module: genbliothinothon->getModuleManager().getModules() | std::views::values) {
			if (module.get() == getParent()) {
				continue;
			}
			const glm::vec2 modulePos = module->getPos();
			std::vector<std::unique_ptr<Part>>& parts = module->getParts();
			for (uint32_t i = 0; i < parts.size(); i++) {
				const std::unique_ptr<Part>& part = parts[i];
				const glm::vec2 partPos = part->getPos() + modulePos;
				const glm::vec2 partSize = part->getSize();
				if (const auto sendTarget = dynamic_cast<SendTarget*>(part.get()); sendTarget && worldPos.x >= partPos.x && worldPos.x < partPos.x + partSize.x && worldPos.y >= partPos.y && worldPos.y < partPos.y + partSize.y) {
					if (!sendTarget->isMultiChannel() && mChannelCount > 1) {
						break;
					}
					mSendTarget = {.module = module->getID(), .part = i};
					Module* parent = getParent();
					uint32_t index = 0;
					const std::vector<std::unique_ptr<Part>>& p = parent->getParts();
					for (uint32_t j = 0; j < p.size(); j++) {
						if (p[j].get() == this) {
							index = j;
							break;
						}
					}
					sendTarget->addInput({.module = parent->getID(), .part = index});
					setDirty(true);
					break;
				}
			}
		}
		mSelecting = false;
	}

	void Send::save(util::FileOStream& stream) {
		Part::save(stream);
		stream << mName;
		stream << mChannelCount;
		const bool hasSendTarget = mSendTarget.has_value();
		stream << hasSendTarget;
		if (hasSendTarget) {
			stream << mSendTarget->module;
			stream << mSendTarget->part;
		}
	}

	void Send::setType(const uint32_t type) {
		sType = type;
	}

	void Send::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rect(pos, size, glm::vec4{0.06f, 0.06f, 0.11f, 1.0f});
		const float textHeight = size.y / 2.0f - 7.0f;
		std::string sendText = "Null (" + mName + ")";
		if (mSendTarget) {
			for (const SendTarget* sendTarget = reinterpret_cast<SendTarget*>(Genbliothinothon::getInstance()->getModuleManager().getModule(mSendTarget->module)->getParts()[mSendTarget->part].get()); const std::unique_ptr<Part>& part: sendTarget->getParent()->getParts()) {
				if (const Background* background = dynamic_cast<Background*>(part.get())) {
					sendText = background->getName() + "." + sendTarget->getName();
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
