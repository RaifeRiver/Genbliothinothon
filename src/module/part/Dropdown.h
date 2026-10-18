#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class Dropdown : public Part {
	public:
		Dropdown(Module* parent, glm::vec2 pos, glm::vec2 size, const std::vector<std::string>& options);

		[[nodiscard]] uint32_t getSelectedOption() const {
			return mSelectedOption;
		}

		void setSelectedOption(uint32_t option);

		[[nodiscard]] const std::vector<std::string>& getOptions() const {
			return mOptions;
		}

		void setOptions(const std::vector<std::string>& options);

		void click(glm::vec2 pos) override;

		void clickOff(glm::vec2 pos) override;

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		std::vector<std::string> mOptions;
		uint32_t mSelectedOption = 0;
		bool mOpen = false;
	};
}
