#include "Part.h"

namespace genbliothinothon::module::part {
	Part::Part(Module* parent, const glm::vec2 pos, const glm::vec2 size) : mParent(parent), mPos(pos), mSize(size) {}

	void Part::render(const glm::vec2 pos, render::RenderContext& context) {
		render_(pos + mPos, context);
	}

	void Part::click(glm::vec2) {}

	void Part::clickOff(glm::vec2) {}

	bool Part::drag(const glm::vec2) {
		return false;
	}

	void Part::endDrag() {}
}
