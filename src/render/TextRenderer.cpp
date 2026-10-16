#include "TextRenderer.h"

#include <fstream>
#include <sstream>

#include "RenderContext.h"
#include "glm/vec3.hpp"

namespace genbliothinothon::render {
	TextRenderer::TextRenderer() {
		mShader = std::make_unique<Shader>("../res/shaders/text.vert", "../res/shaders/text.frag");
		mShader->setUniform("uFontAtlas", 0);

		mPositionBuffer = std::make_unique<Buffer>(12 * MAX_TEXT_VERTICES, GL_DYNAMIC_DRAW);
		mUVBuffer = std::make_unique<Buffer>(8 * MAX_TEXT_VERTICES, GL_DYNAMIC_DRAW);
		mColourBuffer = std::make_unique<Buffer>(16 * MAX_TEXT_VERTICES, GL_DYNAMIC_DRAW);

		mVAO.setAttribute(0, *mPositionBuffer, 3, GL_FLOAT);
		mVAO.setAttribute(1, *mUVBuffer, 2, GL_FLOAT);
		mVAO.setAttribute(2, *mColourBuffer, 4, GL_FLOAT);

		mFontAtlas = std::make_unique<Image>("../res/textures/font.png");
		mFontAtlas->bind(0);

		std::ifstream file("../res/textures/font.csv");
		std::string line;
		while (std::getline(file, line)) {
			std::stringstream stream(line);
			std::string cell;
			std::vector<std::string> row;
			while (std::getline(stream, cell, ',')) {
				row.push_back(cell);
			}
			if (row.size() >= 10) {
				uint32_t unicode = std::stoul(row[0]);
				mFontGlyphs.emplace(unicode, FontGlyph{
					.advance = std::stof(row[1]),
					.atlasBounds = {std::stof(row[6]), std::stof(row[7]), std::stof(row[8]), std::stof(row[9])},
					.planeBounds = {std::stof(row[2]), std::stof(row[3]), std::stof(row[4]), std::stof(row[5])}
				});
			}
		}
	}

	void TextRenderer::text(const glm::vec2 pos, const float size, const std::string& text, const glm::vec4 colour) {
		mText.push_back({.pos = pos, .size = size, .text = text, .colour = colour});
	}

	void TextRenderer::render(RenderContext& context) {
		const auto positionBuffer = static_cast<glm::vec3*>(mPositionBuffer->map());
		const auto uvBuffer = static_cast<glm::vec2*>(mUVBuffer->map());
		const auto colourBuffer = static_cast<glm::vec4*>(mColourBuffer->map());
		int32_t vertexCount = 0;
		const glm::vec2 atlasSize = mFontAtlas->getSize();
		for (auto&[pos, size, text, colour] : mText) {
			glm::vec2 cursor = pos;
			for (const char c : text) {
				auto it = mFontGlyphs.find(c);
				if (it == mFontGlyphs.end()) {
					it = mFontGlyphs.find('?');
					if (it == mFontGlyphs.end()) {
						continue;
					}
				}
				const auto&[advance, atlasBounds, planeBounds] = it->second;

				const float x0 = cursor.x + planeBounds.x * size;
				const float y0 = cursor.y - planeBounds.w * size;
				const float x1 = cursor.x + planeBounds.z * size;
				const float y1 = cursor.y - planeBounds.y * size;

				const float u0 = atlasBounds.x / atlasSize.x;
				const float v0 = 1.0f - atlasBounds.w / atlasSize.y;
				const float u1 = atlasBounds.z / atlasSize.x;
				const float v1 = 1.0f - atlasBounds.y / atlasSize.y;

				positionBuffer[vertexCount] = glm::vec3{x0, y0, 0.0f};
				positionBuffer[vertexCount + 1] = glm::vec3{x1, y0, 0.0f};
				positionBuffer[vertexCount + 2] = glm::vec3{x1, y1, 0.0f};
				positionBuffer[vertexCount + 3] = glm::vec3{x1, y1, 0.0f};
				positionBuffer[vertexCount + 4] = glm::vec3{x0, y1, 0.0f};
				positionBuffer[vertexCount + 5] = glm::vec3{x0, y0, 0.0f};

				uvBuffer[vertexCount] = glm::vec2{u0, v0};
				uvBuffer[vertexCount + 1] = glm::vec2{u1, v0};
				uvBuffer[vertexCount + 2] = glm::vec2{u1, v1};
				uvBuffer[vertexCount + 3] = glm::vec2{u1, v1};
				uvBuffer[vertexCount + 4] = glm::vec2{u0, v1};
				uvBuffer[vertexCount + 5] = glm::vec2{u0, v0};

				colourBuffer[vertexCount] = colour;
				colourBuffer[vertexCount + 1] = colour;
				colourBuffer[vertexCount + 2] = colour;
				colourBuffer[vertexCount + 3] = colour;
				colourBuffer[vertexCount + 4] = colour;
				colourBuffer[vertexCount + 5] = colour;

				vertexCount += 6;
				cursor.x += advance;
			}
		}
		mPositionBuffer->unmap();
		mUVBuffer->unmap();
		mColourBuffer->unmap();
		mText.clear();

		mVAO.bind();
		mShader->bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		context.drawArrays(GL_TRIANGLES, 0, vertexCount);

		glDisable(GL_BLEND);
	}
}
