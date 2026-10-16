#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/vec2.hpp"

namespace genbliothinothon::render {
	class Image {
	public:
		explicit Image(const std::string& path);

		Image(glm::uvec2 size, GLenum format);

		void bind(uint32_t slot) const;

		[[nodiscard]] GLuint getTexture() const {
			return mTexture;
		}

		[[nodiscard]] glm::uvec2 getSize() const {
			return mSize;
		}

		~Image();

	private:
		GLuint mTexture = 0;
		glm::uvec2 mSize = {};
	};
}
