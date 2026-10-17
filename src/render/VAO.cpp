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
		if (type == GL_FLOAT) {
			glVertexAttribPointer(index, size, type, false, 0, nullptr);
		}
		else if (type == GL_UNSIGNED_INT || type == GL_INT) {
			glVertexAttribIPointer(index, size, type, 0, nullptr);
		}
	}

	void VAO::removeAttribute(const GLuint index) const {
		bind();
		glDisableVertexAttribArray(index);
	}

	VAO::~VAO() {
		glDeleteVertexArrays(1, &mVAO);
	}
}
