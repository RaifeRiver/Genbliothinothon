#pragma once

#include "SendTarget.h"

namespace genbliothinothon::module::part {
	class Output : public SendTarget {
	public:
		Output(Module* parent, const glm::vec2& pos, const glm::vec2& size, const std::string& name);

		std::string getDefaultValueString() override;

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;
	};
}
