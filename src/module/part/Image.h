#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class Image : public Part {
	public:
		explicit Image(glm::vec2 pos, glm::vec2 size, std::string texture, glm::vec4 tint = {1.0f, 1.0f, 1.0f, 1.0f});

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		glm::vec2 mSize;
		std::string mTexture;
		glm::vec4 mTint;
	};
}
