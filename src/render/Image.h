#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/vec2.hpp"

namespace genbliothinothon::render {
	class Image {
	public:
		explicit Image(const std::string& path);

		void bind(uint32_t slot) const;

		[[nodiscard]] glm::vec2 getSize() const {
			return mSize;
		}

		~Image();

	private:
		GLuint mTexture = 0;
		glm::vec2 mSize = {};
	};
}
