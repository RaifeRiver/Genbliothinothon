#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class RemoveButton : public Part {
	public:
		explicit RemoveButton(Module* parent);

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;
	};
}