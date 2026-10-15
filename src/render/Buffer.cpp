#include "Buffer.h"

namespace genbliothinothon::render {
	Buffer::Buffer(const void* bufferData, const size_t bufferSize) {
		glGenBuffers(1, &mBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
	}

	Buffer::~Buffer() {
		glDeleteBuffers(1, &mBuffer);
	}
}
