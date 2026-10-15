#include "VAO.h"

namespace genbliothinothon::render {
	VAO::VAO() {
		glGenVertexArrays(1, &mVAO);
	}

	void VAO::bind() const {
		glBindVertexArray(mVAO);
	}

	void VAO::setAttribute(const GLuint index, const Buffer& buffer, const GLint size, const GLenum type) const {
		bind();
		glEnableVertexAttribArray(index);
		glBindBuffer(GL_ARRAY_BUFFER, buffer.getBuffer());
		glVertexAttribPointer(index, size, type, GL_FALSE, 0, nullptr);
	}

	void VAO::removeAttribute(const GLuint index) const {
		bind();
		glDisableVertexAttribArray(index);
	}

	VAO::~VAO() {
		glDeleteVertexArrays(1, &mVAO);
	}
}
