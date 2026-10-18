#include "SendTarget.h"

#include <utility>

namespace genbliothinothon::module::part {
	SendTarget::SendTarget(Module* parent, const glm::vec2 pos, const glm::vec2& size, std::string name) : Part(parent, pos, size), mName(std::move(name)) {}

	void SendTarget::addInput(Send* input) {
		mInputs.push_back(input);
	}
}
