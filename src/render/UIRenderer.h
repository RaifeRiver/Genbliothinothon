#pragma once

#include <memory>
#include <string>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include "Image.h"
#include "Shader.h"
#include "VAO.h"

namespace genbliothinothon::render {
	static constexpr uint32_t MAX_VERTICES = 16384;

	class RenderContext;

	struct Vertex {
		glm::vec2 pos;
		glm::vec2 uv;
		glm::vec4 colour;
		uint32_t type;
	};

	struct FontGlyph {
		float advance;
		glm::vec4 atlasBounds;
		glm::vec4 planeBounds;
	};

	struct UITexture {
		glm::vec2 pos;
		glm::vec2 size;
	};

	class UIRenderer {
	public:
		UIRenderer();

		float getTextWidth(float size, const std::string& text);

		void text(glm::vec2 pos, float size, const std::string& text, glm::vec4 colour);

		void centredText(glm::vec2 pos, float size, const std::string& t, glm::vec4 colour);

		void rect(glm::vec2 pos, glm::vec2 size, glm::vec4 colour, const std::string& texture = "white");

		void rotatedRect(glm::vec2 pos, float angle, glm::vec2 size, glm::vec4 colour, const std::string& texture = "white");

		void render(RenderContext& context);

	private:
		VAO mVAO;
		std::unique_ptr<Shader> mShader;
		std::unique_ptr<Buffer> mPositionBuffer;
		std::unique_ptr<Buffer> mUVBuffer;
		std::unique_ptr<Buffer> mColourBuffer;
		std::unique_ptr<Buffer> mTypeBuffer;
		std::vector<Vertex> mVertices;
		std::unique_ptr<Image> mFontAtlas;
		std::unique_ptr<Image> mUIAtlas;
		std::unordered_map<uint32_t, FontGlyph> mFontGlyphs;
		float mFontAscent = 0.0f;
		std::unordered_map<std::string, UITexture> mUITextures;
	};
}
