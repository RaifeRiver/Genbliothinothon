#include "SendTarget.h"

#include <utility>

namespace genbliothinothon::module::part {
	SendTarget::SendTarget(Module* parent, const glm::vec2 pos, const glm::vec2& size, std::string name, const bool multiChannel) : Part(parent, pos, size), mName(std::move(name)), mMultiChannel(multiChannel) {}

	void SendTarget::addInput(Send* input) {
		mInputs.push_back(input);
	}

	void SendTarget::setMultiChannel(const bool multiChannel) {
		mMultiChannel = multiChannel;
	}

	void SendTarget::setAveraged(const bool averaged) {
		mAveraged = averaged;
	}
}
