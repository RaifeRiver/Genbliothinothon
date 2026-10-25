#include "Module.h"

#include "AHDSR.h"
#include "CSVSequencer.h"
#include "Genbliothinothon.h"
#include "MathsWave.h"
#include "MidiSequencer.h"
#include "NoiseSource.h"
#include "Oscillator.h"
#include "Output.h"
#include "AntiDepressant.h"
#include "BitCrusher.h"
#include "Depresser.h"
#include "FullRectifier.h"
#include "HardClip.h"
#include "OverInvert.h"
#include "SoftDistort.h"
#include "TapeSaturation.h"
#include "Tremlo.h"
#include "TubeSaturation.h"
#include "XOR.h"

namespace genbliothinothon::module {
	Module::Module(const glm::vec2 pos, const glm::vec2 size) : mPos(pos), mSize(size) {}

	Module::Module(const uint32_t version, util::FileIStream& stream) {
		stream >> mID;
		stream >> mPos;
		stream >> mSize;
		uint32_t partCount;
		stream >> partCount;
		mParts.reserve(partCount);
		for (uint32_t i = 0; i < partCount; i++) {
			std::unique_ptr<part::Part> part = part::Part::loadPart(version, stream);
			part->setParent(this);
			mParts.push_back(std::move(part));
		}
	}

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

	void Module::setID(const uint32_t id) {
		mID = id;
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

	std::string Module::generateFunctions(uint32_t) {
		return "";
	}

	void Module::save(util::FileOStream& stream) const {
		stream << mType;
		stream << getID();
		stream << mPos;
		stream << mSize;
		stream << static_cast<uint32_t>(mParts.size());
		for (const std::unique_ptr<part::Part>& part: mParts) {
			part->save(stream);
		}
	}

	void Module::render(glm::vec2 pos, render::RenderContext& context) const {
		pos += mPos;
		for (const std::unique_ptr<part::Part>& part: mParts) {
			part->render(pos, context);
		}
	}

	std::unique_ptr<Module> Module::createModule(const std::string& type, const glm::vec2 pos) {
		initModules();
		for (const auto& [t, constructor, _]: sModuleRegistry) {
			if (type == t) {
				return constructor(pos);
			}
		}
		return nullptr;
	}

	std::unique_ptr<Module> Module::loadModule(const uint32_t version, util::FileIStream& stream) {
		initModules();
		uint32_t type;
		stream >> type;
		std::unique_ptr<Module> module = std::get<2>(sModuleRegistry[type])(version, stream);
		module->mType = type;
		return std::move(module);
	}

	void Module::initModules() {
		static uint32_t _ = registerModules();
	}

	uint32_t Module::registerModules() {
		registerModule<AHDSR>("AHDSR");
		registerModule<CSVSequencer>("CSV Sequencer");
		registerModule<MathsWave>("Maths Wave");
		registerModule<MidiSequencer>("MIDI Sequencer");
		registerModule<NoiseSource>("Noise Source");
		registerModule<Oscillator>("Oscillator");
		registerModule<Output>("Output");
		registerModule<AntiDepressant>("AntiDepressant");
		registerModule<BitCrusher>("BitCrusher");
		registerModule<Depresser>("Depresser");
		registerModule<FullRectifier>("FullRectifier");
		registerModule<HardClip>("HardClip");
		registerModule<OverInvert>("OverInvert");
		registerModule<SoftDistort>("SoftDistort");
		registerModule<TapeSaturation>("TapeSaturation");
		registerModule<Tremlo>("Tremlo");
		registerModule<TubeSaturation>("TubeSaturation");
		registerModule<XOR>("XOR");
		return 0;
	}
}
