#pragma once

#include "Part.h"
#include "module/Module.h"

namespace genbliothinothon::module::part {
	class Send : public Part {
	public:
		Send(const glm::vec2& pos, const glm::vec2& size);

		[[nodiscard]] Module* getSendTarget() const {
			return mSendTarget;
		}

		void setSendTarget(Module* target);

		void click(glm::vec2 pos) override;

		void clickOff(glm::vec2 pos) override;

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		Module* mSendTarget = nullptr;
		bool mSelecting = false;
	};
}
