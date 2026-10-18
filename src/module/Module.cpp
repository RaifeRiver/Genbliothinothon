#include "Module.h"

#include "Genbliothinothon.h"

namespace genbliothinothon::module {
	Module::Module(const glm::vec2 pos, const glm::vec2 size) : mPos(pos), mSize(size) {}

	void Module::setPos(const glm::vec2 pos) {
		mPos = pos;
	}

	void Module::setSize(const glm::vec2 size) {
		mSize = size;
	}

	bool Module::isDirty() const {
		if (mDirty) {
			return true;
		}
		for (const std::unique_ptr<part::Part>& part: mParts) {
			if (part->isDirty()) {
				return true;
			}
		}
		return false;
	}

	void Module::clearDirty() {
		for (const std::unique_ptr<part::Part>& part: mParts) {
			part->setDirty(false);
		}
		mDirty = false;
	}

	void Module::setDirty() {
		mDirty = true;
	}

	void Module::drag(const glm::vec2 distance) {
		bool override = false;
		Genbliothinothon* genbliothinothon = Genbliothinothon::getInstance();
		const glm::vec2 worldPos = genbliothinothon->getRenderContext().getMousePos() - genbliothinothon->getCameraPos();
		for (const std::unique_ptr<part::Part>& part: mParts) {
			if (part->isDragged()) {
				override |= part->drag(distance);
				if (override) {
					break;
				}
			}
			const glm::vec2 pos = part->getPos() + mPos;
			if (const glm::vec2 size = part->getSize(); worldPos.x >= pos.x && worldPos.x < pos.x + size.x && worldPos.y >= pos.y && worldPos.y < pos.y + size.y) {
				part->setDragged(true);
				override |= part->drag(distance);
				if (override) {
					break;
				}
			}
		}
		if (!override) {
			mPos += distance;
		}
	}

	void Module::endDrag() const {
		for (const std::unique_ptr<part::Part>& part: mParts) {
			if (part->isDragged()) {
				part->endDrag();
				part->setDragged(false);
			}
		}
	}

	std::string Module::generateFunctions(uint32_t index) {
		return "";
	}

	void Module::render(glm::vec2 pos, render::RenderContext& context) const {
		pos += mPos;
		for (const std::unique_ptr<part::Part>& part: mParts) {
			part->render(pos, context);
		}
	}
}
