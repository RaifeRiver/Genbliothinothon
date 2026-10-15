#include "Buffer.h"

namespace genbliothinothon::render {
	Buffer::Buffer(const void* bufferData, const GLsizeiptr bufferSize) {
		glGenBuffers(1, &mBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
	}

	Buffer::~Buffer() {
		glDeleteBuffers(1, &mBuffer);
	}
}
