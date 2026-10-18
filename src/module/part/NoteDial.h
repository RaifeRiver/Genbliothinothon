#pragma once

#include "Dial.h"

namespace genbliothinothon::module::part {
	class NoteDial : public Dial {
	public:
		NoteDial(Module* parent, const glm::vec2& pos, const glm::vec2& size, const std::string& name);

		std::string getDefaultValueString() override;

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;
	};
}
