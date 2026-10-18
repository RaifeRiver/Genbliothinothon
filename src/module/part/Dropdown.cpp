#include "Dropdown.h"

namespace genbliothinothon::module::part {
	Dropdown::Dropdown(Module* parent, const glm::vec2 pos, const glm::vec2 size, const std::vector<std::string>& options) : Part(parent, pos, size), mOptions(options) {}

	void Dropdown::setSelectedOption(const uint32_t option) {
		if (option >= mOptions.size()) {
			throw std::out_of_range("Option index out of range");
		}
		mSelectedOption = option;
	}

	void Dropdown::setOptions(const std::vector<std::string>& options) {
		mOptions = options;
		if (mSelectedOption >= mOptions.size()) {
			mSelectedOption = 0;
		}
		setDirty(true);
	}

	void Dropdown::click(glm::vec2) {
		mOpen = !mOpen;
	}

	void Dropdown::clickOff(const glm::vec2 pos) {
		if (mOpen) {
			if (const glm::vec2 size = getSize(); pos.y > size.y && pos.y <= size.y * static_cast<float>(mOptions.size() + 1)) {
				mSelectedOption = static_cast<uint32_t>((pos.y - size.y) / size.y);
				setDirty(true);
			}
			mOpen = false;
		}
	}

	void Dropdown::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rect(pos, size, glm::vec4{0.06f, 0.06f, 0.11f, 1.0f});
		const float textHeight = size.y / 2.0f - 7.0f;
		renderer.text(pos + glm::vec2{2.0f, textHeight}, 14.0f, mOptions[mSelectedOption], glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
		if (mOpen) {
			renderer.rect(pos + glm::vec2{0.0f, size.y}, {size.x, size.y * static_cast<float>(mOptions.size())}, glm::vec4{0.07f, 0.07f, 0.13f, 1.0f});
			for (uint32_t option = 0; option < mOptions.size(); option++) {
				if (option == mSelectedOption) {
					renderer.rect(pos + glm::vec2{0.0f, size.y * static_cast<float>(option + 1)}, {size.x, size.y}, glm::vec4{0.1f, 0.1f, 0.18f, 1.0f});
				}
				renderer.text(pos + glm::vec2{2.0f, (static_cast<float>(option + 1)) * size.y}, 14.0f, mOptions[option], {1.0f, 1.0f, 1.0f, 1.0f});
			}
		}
	}
}
