#include "Module.h"

namespace genbliothinothon::module {
	Module::Module(const glm::vec2 pos, const glm::vec2 size) : mPos(pos), mSize(size) {}

	void Module::setPos(const glm::vec2 pos) {
		mPos = pos;
	}

	void Module::setSize(const glm::vec2 size) {
		mSize = size;
	}

	bool Module::isDirty() const {
		for (const std::unique_ptr<part::Part>& part: mParts) {
			if (part->isDirty()) {
				return true;
			}
		}
		return false;
	}

	void Module::clearDirty() const {
		for (const std::unique_ptr<part::Part>& part: mParts) {
			part->setDirty(false);
		}
	}

	void Module::render(glm::vec2 pos, render::RenderContext& context) const {
		pos += mPos;
		for (const std::unique_ptr<part::Part>& part: mParts) {
			part->render(pos, context);
		}
	}
}
