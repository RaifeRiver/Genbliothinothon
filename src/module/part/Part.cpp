#include "Part.h"

namespace genbliothinothon::module::part {
	Part::Part(const glm::vec2 pos, const glm::vec2 size) : mPos(pos), mSize(size) {}

	void Part::render(const glm::vec2 pos, render::RenderContext& context) {
		render_(pos + mPos, context);
	}

	void Part::click(glm::vec2) {}

	void Part::clickOff(glm::vec2) {}
}
