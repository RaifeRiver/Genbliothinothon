#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class Dropdown : public Part {
	public:
		Dropdown(Module* parent, glm::vec2 pos, glm::vec2 size, const std::vector<std::string>& options);

		Dropdown(uint32_t version, util::FileIStream& stream);

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

		void save(util::FileOStream& stream) override;

		static void setType(uint32_t type);

		[[nodiscard]] static uint32_t getType() {
			return sType;
		}

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		static inline uint32_t sType;
		std::vector<std::string> mOptions;
		uint32_t mSelectedOption = 0;
		bool mOpen = false;
	};
}
