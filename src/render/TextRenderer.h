#pragma once

#include <memory>
#include <string>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include "Image.h"
#include "Shader.h"
#include "VAO.h"

namespace genbliothinothon::render {
	static constexpr uint32_t MAX_TEXT_VERTICES = 16384;

	class RenderContext;

	struct Text {
		glm::vec2 pos;
		float size;
		std::string text;
		glm::vec4 colour;
	};

	struct FontGlyph {
		float advance;
		glm::vec4 atlasBounds;
		glm::vec4 planeBounds;
	};

	class TextRenderer {
	public:
		TextRenderer();

		void text(glm::vec2 pos, float size, const std::string& text, glm::vec4 colour);

		void render(RenderContext& context);

	private:
		VAO mVAO;
		std::unique_ptr<Shader> mShader;
		std::unique_ptr<Buffer> mPositionBuffer;
		std::unique_ptr<Buffer> mUVBuffer;
		std::unique_ptr<Buffer> mColourBuffer;
		std::vector<Text> mText;
		std::unique_ptr<Image> mFontAtlas;
		std::unordered_map<uint32_t, FontGlyph> mFontGlyphs;
	};
}
