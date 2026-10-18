#pragma once

#include <chrono>

#include "Part.h"

namespace genbliothinothon::module::part {
	class TextInput : public Part {
	public:
		TextInput(Module* parent, const glm::vec2& pos, const glm::vec2& size);

		[[nodiscard]] const std::string& getText() const {
			return mText;
		}

		void setText(const std::string& text);

		void click(glm::vec2 pos) override;

		void clickOff(glm::vec2 pos) override;

		void charPressed(char key) override;

		void keyPressed(int key) override;

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		std::string mText;
		bool mSelected = false;
		std::chrono::steady_clock::time_point mCursorTime;
	};
}
