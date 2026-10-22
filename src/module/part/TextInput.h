#pragma once

#include <chrono>

#include "Part.h"

namespace genbliothinothon::module::part {
	class TextInput : public Part {
	public:
		TextInput(Module* parent, glm::vec2 pos, glm::vec2 size);

		TextInput(uint32_t version, util::FileIStream& stream);

		[[nodiscard]] const std::string& getText() const {
			return mText;
		}

		void setText(const std::string& text);

		void click(glm::vec2 pos) override;

		void clickOff(glm::vec2 pos) override;

		void charPressed(char key) override;

		void keyPressed(int key) override;

		void save(util::FileOStream& stream) override;

		static void setType(uint32_t type);

		[[nodiscard]] static uint32_t getType() {
			return sType;
		}

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		static inline uint32_t sType;
		std::string mText;
		bool mSelected = false;
		std::chrono::steady_clock::time_point mCursorTime;
	};
}
