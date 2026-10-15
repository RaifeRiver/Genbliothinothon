#pragma once

#include "glad/glad.h"

namespace genbliothinothon::render {
	class Buffer {
	public:
		Buffer(const void* bufferData, GLsizeiptr bufferSize);

		[[nodiscard]] GLuint getBuffer() const {
			return mBuffer;
		}

		~Buffer();

	private:
		GLuint mBuffer = 0;
	};
}
