#include "SendTarget.h"

#include <utility>

namespace genbliothinothon::module::part {
	SendTarget::SendTarget(const uint32_t type, Module* parent, const glm::vec2 pos, const glm::vec2& size, std::string name, const bool multiChannel) : Part(type, parent, pos, size), mName(std::move(name)), mMultiChannel(multiChannel) {}

	SendTarget::SendTarget(const uint32_t version, util::FileIStream& stream) : Part(version, stream) {
		stream >> mName;
		stream >> mMultiChannel;
		if (version == 0) {
			bool averaged;
			stream >> averaged;
			if (!averaged) {
				mCombiningMode = CombiningMode::SUM;
			}
		}
		else {
			stream >> reinterpret_cast<uint32_t&>(mCombiningMode);
		}
		uint32_t inputCount;
		stream >> inputCount;
		mInputs.reserve(inputCount);
		for (uint32_t i = 0; i < inputCount; i++) {
			PartID input = {};
			stream >> input.module;
			stream >> input.part;
			mInputs.push_back(input);
		}
	}

	void SendTarget::addInput(const PartID input) {
		mInputs.push_back(input);
	}

	void SendTarget::setMultiChannel(const bool multiChannel) {
		mMultiChannel = multiChannel;
	}

	void SendTarget::setCombiningMode(const CombiningMode combiningMode) {
		mCombiningMode = combiningMode;
	}

	void SendTarget::save(util::FileOStream& stream) {
		Part::save(stream);
		stream << mName;
		stream << mMultiChannel;
		stream << static_cast<uint32_t>(mCombiningMode);
		stream << static_cast<uint32_t>(mInputs.size());
		for (const auto& [module, part]: mInputs) {
			stream << module;
			stream << part;
		}
	}
}
