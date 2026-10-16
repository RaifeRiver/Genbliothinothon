#include "Buffer.h"

namespace genbliothinothon::render {
	Buffer::Buffer(const uint32_t bufferSize, const GLenum mode) : mSize(bufferSize) {
		glGenBuffers(1, &mBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, mode);
	}

	Buffer::Buffer(const void* bufferData, const uint32_t bufferSize, const GLenum mode) : mSize(bufferSize) {
		glGenBuffers(1, &mBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, mode);
	}

	void* Buffer::map() const {
		glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
		return glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	}

	void Buffer::unmap() const {
		glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	Buffer::~Buffer() {
		glDeleteBuffers(1, &mBuffer);
	}
}
