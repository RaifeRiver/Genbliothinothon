#pragma once

#include "SendTarget.h"

namespace genbliothinothon::module::part {
	class GenericSendTarget : public SendTarget {
	public:
		GenericSendTarget(Module* parent, glm::vec2 pos, glm::vec2 size, const std::string& name, const std::string& defaultValue, bool multiChannel = false);

		[[nodiscard]] std::string getDefaultValueString() override {
			return mDefaultValue;
		}

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		std::string mDefaultValue;
	};
}
