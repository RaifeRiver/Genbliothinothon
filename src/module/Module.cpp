#include "Module.h"

namespace genbliothinothon::module {
	Module::Module(const glm::vec2 pos) : mPos(pos) {}

	part::Part& Module::addPart(std::unique_ptr<part::Part> part) {
		mParts.push_back(std::move(part));
		return *mParts.back();
	}

	void Module::setPos(const glm::vec2 pos) {
		mPos = pos;
	}

	void Module::render(glm::vec2 pos, render::RenderContext& context) const {
		pos += mPos;
		for (const std::unique_ptr<part::Part>& part: mParts) {
			part->render(pos, context);
		}
	}
}
