#include "UIRenderer.h"

#include <fstream>
#include <sstream>

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/transform.hpp"

#include "RenderContext.h"

namespace genbliothinothon::render {
	UIRenderer::UIRenderer() {
		mShader = Shader::loadFromFiles("../res/shaders/ui.vert", "../res/shaders/ui.frag");
		mShader->setUniform("uFontAtlas", 0);
		mShader->setUniform("uUIAtlas", 1);

		mPositionBuffer = std::make_unique<Buffer>(8 * MAX_VERTICES, GL_DYNAMIC_DRAW);
		mUVBuffer = std::make_unique<Buffer>(8 * MAX_VERTICES, GL_DYNAMIC_DRAW);
		mColourBuffer = std::make_unique<Buffer>(16 * MAX_VERTICES, GL_DYNAMIC_DRAW);
		mTypeBuffer = std::make_unique<Buffer>(4 * MAX_VERTICES, GL_DYNAMIC_DRAW);

		mVAO.setAttribute(0, *mPositionBuffer, 2, GL_FLOAT);
		mVAO.setAttribute(1, *mUVBuffer, 2, GL_FLOAT);
		mVAO.setAttribute(2, *mColourBuffer, 4, GL_FLOAT);
		mVAO.setAttribute(3, *mTypeBuffer, 1, GL_UNSIGNED_INT);

		mFontAtlas = std::make_unique<Image>("../res/textures/font.png");
		mUIAtlas = std::make_unique<Image>("../res/textures/ui.png");

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
				mFontAscent = std::max(mFontAscent, mFontGlyphs[unicode].planeBounds.w);
			}
		}
		std::ifstream uiFile("../res/textures/ui.csv");
		glm::vec2 atlasSize = mUIAtlas->getSize();
		glm::vec2 pixelSize = 1.0f / atlasSize;
		glm::vec2 halfPixelSize = pixelSize * 0.5f;;
		while (std::getline(uiFile, line)) {
			std::stringstream stream(line);
			std::string cell;
			std::vector<std::string> row;
			while (std::getline(stream, cell, ',')) {
				row.push_back(cell);
			}
			if (row.size() >= 5) {
				mUITextures[row[0]] = {
					.pos = {std::stof(row[1]) / atlasSize.x + halfPixelSize.x, std::stof(row[2]) / atlasSize.y + halfPixelSize.y},
					.size = {std::stof(row[3]) / atlasSize.x - pixelSize.x, std::stof(row[4]) / atlasSize.y - pixelSize.y}
				};
			}
		}
	}

	void UIRenderer::text(const glm::vec2 pos, const float size, const std::string& text, const glm::vec4 colour) {
		glm::vec2 cursor = pos;
		cursor.y += mFontAscent * size;
		const glm::vec2 atlasSize = mFontAtlas->getSize();
		for (const char c : text) {
			auto it = mFontGlyphs.find(c);
			if (it == mFontGlyphs.end()) {
				it = mFontGlyphs.find('?');
				if (it == mFontGlyphs.end()) {
					continue;
				}
			}
			const auto& [advance, atlasBounds, planeBounds] = it->second;

			const float x0 = cursor.x + planeBounds.x * size;
			const float y0 = cursor.y - planeBounds.w * size;
			const float x1 = cursor.x + planeBounds.z * size;
			const float y1 = cursor.y - planeBounds.y * size;

			const float u0 = atlasBounds.x / atlasSize.x;
			const float v0 = 1.0f - atlasBounds.w / atlasSize.y;
			const float u1 = atlasBounds.z / atlasSize.x;
			const float v1 = 1.0f - atlasBounds.y / atlasSize.y;

			mVertices.emplace_back(glm::vec2{x0, y0}, glm::vec2{u0, v0}, colour, 0);
			mVertices.emplace_back(glm::vec2{x1, y0}, glm::vec2{u1, v0}, colour, 0);
			mVertices.emplace_back(glm::vec2{x1, y1}, glm::vec2{u1, v1}, colour, 0);
			mVertices.emplace_back(glm::vec2{x1, y1}, glm::vec2{u1, v1}, colour, 0);
			mVertices.emplace_back(glm::vec2{x0, y1}, glm::vec2{u0, v1}, colour, 0);
			mVertices.emplace_back(glm::vec2{x0, y0}, glm::vec2{u0, v0}, colour, 0);
			
			cursor.x += advance * size;
		}
	}

	void UIRenderer::centredText(glm::vec2 pos, const float size, const std::string& t, const glm::vec4 colour) {
		float width = 0.0f;
		for (const char c : t) {
			auto it = mFontGlyphs.find(c);
			if (it == mFontGlyphs.end()) {
				it = mFontGlyphs.find('?');
				if (it == mFontGlyphs.end()) {
					continue;
				}
			}
			width += it->second.advance * size;
		}
		pos.x -= width * 0.5f;
		text(pos, size, t, colour);
	}

	void UIRenderer::rect(glm::vec2 pos, const glm::vec2 size, glm::vec4 colour, const std::string& texture) {
		auto it = mUITextures.find(texture);
		if (it == mUITextures.end()) {
			it = mUITextures.find("white");
		}
		auto& [atlasPos, atlasSize] = it->second;
		mVertices.emplace_back(pos, atlasPos, colour, 1);
		mVertices.emplace_back(pos + glm::vec2{size.x, 0.0f}, atlasPos + glm::vec2{atlasSize.x, 0.0f}, colour, 1);
		mVertices.emplace_back(pos + glm::vec2{size.x, size.y}, atlasPos + glm::vec2{atlasSize.x, atlasSize.y}, colour, 1);
		mVertices.emplace_back(pos + glm::vec2{size.x, size.y}, atlasPos + glm::vec2{atlasSize.x, atlasSize.y}, colour, 1);
		mVertices.emplace_back(pos + glm::vec2{0.0f, size.y}, atlasPos + glm::vec2{0.0f, atlasSize.y}, colour, 1);
		mVertices.emplace_back(pos, atlasPos, colour, 1);
	}

	void UIRenderer::rotatedRect(const glm::vec2 pos, const float angle, const glm::vec2 size, glm::vec4 colour, const std::string& texture) {
		auto it = mUITextures.find(texture);
		if (it == mUITextures.end()) {
			it = mUITextures.find("white");
		}
		auto& [atlasPos, atlasSize] = it->second;
		const glm::vec2 halfSize = size / 2.0f;
		const glm::vec2 centre = pos + halfSize;
		const glm::mat4 model = glm::rotate(glm::translate(glm::vec3(centre, 0.0f)), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		mVertices.emplace_back(glm::vec2(model * glm::vec4(-halfSize, 0.0f, 1.0f)), atlasPos, colour, 1);
		mVertices.emplace_back(glm::vec2(model * glm::vec4(glm::vec2{halfSize.x, -halfSize.y}, 0.0f, 1.0f)), atlasPos + glm::vec2{atlasSize.x, 0.0f}, colour, 1);
		mVertices.emplace_back(glm::vec2(model * glm::vec4(halfSize, 0.0f, 1.0f)), atlasPos + glm::vec2{atlasSize.x, atlasSize.y}, colour, 1);
		mVertices.emplace_back(glm::vec2(model * glm::vec4(halfSize, 0.0f, 1.0f)), atlasPos + glm::vec2{atlasSize.x, atlasSize.y}, colour, 1);
		mVertices.emplace_back(glm::vec2(model * glm::vec4(glm::vec2{-halfSize.x, halfSize.y}, 0.0f, 1.0f)), atlasPos + glm::vec2{0.0f, atlasSize.y}, colour, 1);
		mVertices.emplace_back(glm::vec2(model * glm::vec4(-halfSize, 0.0f, 1.0f)), atlasPos, colour, 1);
	}

	void UIRenderer::render(RenderContext& context) {
		const auto positionBuffer = static_cast<glm::vec2*>(mPositionBuffer->map());
		const auto uvBuffer = static_cast<glm::vec2*>(mUVBuffer->map());
		const auto colourBuffer = static_cast<glm::vec4*>(mColourBuffer->map());
		const auto typeBuffer = static_cast<uint32_t*>(mTypeBuffer->map());
		const uint32_t vertexCount = mVertices.size();
		for (uint32_t vertex = 0; vertex < vertexCount; vertex++) {
			positionBuffer[vertex] = mVertices[vertex].pos;
			uvBuffer[vertex] = mVertices[vertex].uv;
			colourBuffer[vertex] = mVertices[vertex].colour;
			typeBuffer[vertex] = mVertices[vertex].type;
		}
		mPositionBuffer->unmap();
		mUVBuffer->unmap();
		mColourBuffer->unmap();
		mTypeBuffer->unmap();
		mVertices.clear();

		mVAO.bind();
		mShader->bind();
		mFontAtlas->bind(0);
		mUIAtlas->bind(1);

		mShader->setUniform("uScreenSize", context.getWindowSize());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		context.drawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));

		glDisable(GL_BLEND);
	}
}
