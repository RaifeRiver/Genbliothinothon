#include "Part.h"

#include "Background.h"
#include "Dial.h"
#include "Dropdown.h"
#include "GenericSendTarget.h"
#include "Image.h"
#include "NoteDial.h"
#include "Output.h"
#include "RemoveButton.h"
#include "Send.h"
#include "Text.h"
#include "TextInput.h"

namespace genbliothinothon::module::part {
	bool operator==(const PartID a, const PartID b) {
		return a.module == b.module && a.part == b.part;
	}

	Part::Part(const uint32_t type, Module* parent, const glm::vec2 pos, const glm::vec2 size) : mParent(parent), mPos(pos), mSize(size), mType(type) {}

	Part::Part(uint32_t, util::FileIStream& stream) {
		stream >> mPos;
		stream >> mSize;
	}

	void Part::render(const glm::vec2 pos, render::RenderContext& context) {
		render_(pos + mPos, context);
	}

	void Part::setParent(Module* parent) {
		mParent = parent;
	}

	void Part::setPos(const glm::vec2 pos) {
		mPos = pos;
	}

	void Part::setSize(const glm::vec2 size) {
		mSize = size;
	}

	void Part::setType(const uint32_t type) {
		mType = type;
	}

	void Part::click(glm::vec2) {}

	void Part::clickOff(glm::vec2, glm::vec2) {}

	void Part::drag(const glm::vec2) {}

	void Part::endDrag() {}

	void Part::charPressed(char) {}

	void Part::keyPressed(int) {}

	void Part::save(util::FileOStream& stream) {
		stream << mType;
		stream << mPos;
		stream << mSize;
	}

	std::unique_ptr<Part> Part::loadPart(const uint32_t version, util::FileIStream& stream) {
		initParts();
		uint32_t type;
		stream >> type;
		std::unique_ptr<Part> part = sPartRegistry[type](version, stream);
		part->mType = type;
		return std::move(part);
	}

	void Part::initParts() {
		static uint32_t _ = registerParts();
	}

	uint32_t Part::registerParts() {
		registerPart<Background>();
		registerPart<Dial>();
		registerPart<Dropdown>();
		registerPart<GenericSendTarget>();
		registerPart<Image>();
		registerPart<NoteDial>();
		registerPart<Output>();
		registerPart<RemoveButton>();
		registerPart<Send>();
		registerPart<Text>();
		registerPart<TextInput>();
		return 0;
	}
}
