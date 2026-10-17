#include "Part.h"

namespace genbliothinothon::module::part {
	Part::Part(const glm::vec2 pos) : mPos(pos) {}

	void Part::render(const glm::vec2 pos, render::RenderContext& context) {
		render_(pos + mPos, context);
	}
}
