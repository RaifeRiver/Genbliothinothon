#include "NoteDial.h"

#include <cmath>
#include <format>

namespace genbliothinothon::module::part {
	NoteDial::NoteDial(Module* parent, const glm::vec2& pos, const glm::vec2& size, const std::string& name, const bool multiChannel) : Dial(parent, pos, size, name, 1, 120, 60, multiChannel) {}

	std::string NoteDial::getDefaultValueString() {
		return std::to_string(27.5f * std::pow(2.0f, (std::round(getValue()) - 1.0f) / 12.0f)) + "f";
	}

	void NoteDial::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		const float value = getValue();
		const float min = getMin();
		const float max = getMax();
		renderer.rotatedRect(pos, (value - min) / (max - min) * 300.0f - 150.0f, size, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, "dial");
		const auto note = static_cast<uint32_t>(value);
		renderer.centredText(pos + glm::vec2{size.x * 0.5f, size.y * 0.9f}, 12.0f, isDragged() ? std::to_string(static_cast<int32_t>(note) - 60) : getName(), glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
	}
}
