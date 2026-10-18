#include "Dial.h"

#include <algorithm>

namespace genbliothinothon::module::part {
	Dial::Dial(Module* parent, const glm::vec2 pos, const glm::vec2 size, const std::string& name, const float min, const float max) : Dial(parent, pos, size, name, min, max, min) {}

	Dial::Dial(Module* parent, const glm::vec2 pos, const glm::vec2 size, const std::string& name, const float min, const float max, const float initial) : SendTarget(parent, pos, size, name), mMin(min), mMax(max), mValue(initial) {}

	std::string Dial::getDefaultValueString() {
		return std::to_string(mValue) + "f";
	}

	bool Dial::drag(const glm::vec2 distance) {
		mValue -= (distance.y - distance.x) * (mMax - mMin) * 0.0001f;
		mValue = std::clamp(mValue, mMin, mMax);
		return true;
	}

	void Dial::endDrag() {
		setDirty(true);
	}

	void Dial::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rect(pos, size, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "dial");
	}
}
