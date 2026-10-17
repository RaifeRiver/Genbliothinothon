#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class Text : public Part {
	public:
		Text(glm::vec2 pos, float size, std::string text, glm::vec4 colour = {1.0f, 1.0f, 1.0f, 1.0f});

	protected:
		void render_(glm::vec2 pos, render::RenderContext &context) override;

	private:
		float mSize;
		std::string mText;
		glm::vec4 mColour;
	};
}
