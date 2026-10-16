#pragma once

#include "glad/glad.h"

namespace genbliothinothon::render {
	class Buffer {
	public:
		Buffer(uint32_t bufferSize, GLenum mode = GL_STATIC_DRAW);

		Buffer(const void* bufferData, uint32_t bufferSize, GLenum mode = GL_STATIC_DRAW);

		[[nodiscard]] void* map() const;

		void unmap() const;

		[[nodiscard]] GLuint getBuffer() const {
			return mBuffer;
		}

		[[nodiscard]] uint32_t getSize() const {
			return mSize;
		}

		~Buffer();

	private:
		GLuint mBuffer = 0;
		uint32_t mSize;
	};
}
