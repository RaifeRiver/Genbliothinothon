#pragma once

#include <cstddef>

#include "glad/glad.h"

namespace genbliothinothon::render {
	class Buffer {
	public:
		Buffer(const void* bufferData, size_t bufferSize);

		[[nodiscard]] GLuint getBuffer() const {
			return mBuffer;
		}

		~Buffer();

	private:
		GLuint mBuffer;
	};
}
