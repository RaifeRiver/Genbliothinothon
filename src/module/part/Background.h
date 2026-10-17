#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class Background : public Part {
	public:
		explicit Background(glm::vec2 pos, glm::vec2 size);

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		glm::vec2 mSize;
	};
}
