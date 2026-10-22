#include "SendTarget.h"

#include <utility>

namespace genbliothinothon::module::part {
	SendTarget::SendTarget(const uint32_t type, Module* parent, const glm::vec2 pos, const glm::vec2& size, std::string name, const bool multiChannel) : Part(type, parent, pos, size), mName(std::move(name)), mMultiChannel(multiChannel) {}

	SendTarget::SendTarget(const uint32_t version, util::FileIStream& stream) : Part(version, stream) {
		stream >> mName;
		stream >> mMultiChannel;
		stream >> mAveraged;
	}

	void SendTarget::addInput(Send* input) {
		mInputs.push_back(input);
	}

	void SendTarget::setMultiChannel(const bool multiChannel) {
		mMultiChannel = multiChannel;
	}

	void SendTarget::setAveraged(const bool averaged) {
		mAveraged = averaged;
	}

	void SendTarget::save(util::FileOStream& stream) {
		Part::save(stream);
		stream << mName;
		stream << mMultiChannel;
		stream << mAveraged;
	}
}
