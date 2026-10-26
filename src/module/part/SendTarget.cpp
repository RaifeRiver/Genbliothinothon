#include "SendTarget.h"

#include <utility>

namespace genbliothinothon::module::part {
	SendTarget::SendTarget(const uint32_t type, Module* parent, const glm::vec2 pos, const glm::vec2& size, std::string name) : Part(type, parent, pos, size), mName(std::move(name)) {}

	SendTarget::SendTarget(const uint32_t version, util::FileIStream& stream) : Part(version, stream) {
		stream >> mName;
		stream >> reinterpret_cast<uint32_t&>(mCombiningMode);
		bool hasInput;
		stream >> hasInput;
		if (hasInput) {
			mInput = {.module = 0, .part = 0};
			stream >> mInput->module;
			stream >> mInput->part;
		}
	}

	SendTarget& SendTarget::setInput(const std::optional<PartID> input) {
		mInput = input;
		if (mInputCallback.has_value()) {
			mInputCallback.value()();
		}
		return *this;
	}

	SendTarget& SendTarget::setCombiningMode(const CombiningMode combiningMode) {
		mCombiningMode = combiningMode;
		return *this;
	}

	SendTarget& SendTarget::setInputCallback(const std::optional<std::function<void()>>& callback) {
		mInputCallback = callback;
		return *this;
	}

	void SendTarget::updateInputs() const {
		if (mInputCallback.has_value()) {
			mInputCallback.value()();
		}
	}

	void SendTarget::save(util::FileOStream& stream) {
		Part::save(stream);
		stream << mName;
		stream << static_cast<uint32_t>(mCombiningMode);
		const bool hasInput = mInput.has_value();
		stream << hasInput;
		if (hasInput) {
			stream << mInput->module;
			stream << mInput->part;
		}
	}
}
