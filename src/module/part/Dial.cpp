#include "Dial.h"

#include <algorithm>
#include <format>

namespace genbliothinothon::module::part {
	Dial::Dial(Module* parent, const glm::vec2 pos, const glm::vec2 size, const std::string& name, const float min, const float max, const bool multiChannel) : Dial(parent, pos, size, name, min, max, min, multiChannel) {}

	Dial::Dial(Module* parent, const glm::vec2 pos, const glm::vec2 size, const std::string& name, const float min, const float max, const float initial, const bool multiChannel) : SendTarget(sType, parent, pos, size, name, multiChannel), mMin(min), mMax(max), mValue(initial) {}

	Dial::Dial(const uint32_t version, util::FileIStream& stream) : SendTarget(version, stream) {
		stream >> mMin;
		stream >> mMax;
		stream >> mValue;
	}

	std::string Dial::getDefaultValueString() {
		return std::to_string(mValue) + "f";
	}

	bool Dial::drag(const glm::vec2 distance) {
		mValue -= (distance.y - distance.x) * (mMax - mMin) * 0.001f;
		mValue = std::clamp(mValue, mMin, mMax);
		return true;
	}

	void Dial::endDrag() {
		setDirty(true);
	}

	void Dial::setValue(const float value) {
		mValue = value;
	}

	void Dial::setMin(const float min) {
		mMin = min;
	}

	void Dial::setMax(const float max) {
		mMax = max;
	}

	void Dial::save(util::FileOStream& stream) {
		SendTarget::save(stream);
		stream << mMin;
		stream << mMax;
		stream << mValue;
	}

	void Dial::setType(const uint32_t type) {
		sType = type;
	}

	void Dial::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rotatedRect(pos, (mValue - mMin) / (mMax - mMin) * 300.0f - 150.0f, size, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "dial");
		renderer.centredText(pos + glm::vec2{size.x * 0.5f, size.y * 0.9f}, 12.0f, isDragged() ? std::format("{:.2f}", mValue) : getName(), glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
	}
}
