#pragma once

#include "Part.h"
#include "SendTarget.h"

namespace genbliothinothon::module::part {
	class Send : public Part {
	public:
		Send(Module* parent, const glm::vec2& pos, const glm::vec2& size, std::string name, uint32_t channelCount = 1);

		[[nodiscard]] SendTarget* getSendTarget() const {
			return mSendTarget;
		}

		void setSendTarget(SendTarget* target);

		[[nodiscard]] std::string getName() const {
			return mName;
		}

		[[nodiscard]] uint32_t getChannelCount() const {
			return mChannelCount;
		}

		void setChannelCount(uint32_t channelCount);

		void click(glm::vec2 pos) override;

		void clickOff(glm::vec2 pos) override;

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		SendTarget* mSendTarget = nullptr;
		bool mSelecting = false;
		std::string mName;
		uint32_t mChannelCount;
	};
}
