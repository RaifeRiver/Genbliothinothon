#pragma once

#include "glad/glad.h"

#include "Buffer.h"

namespace genbliothinothon::render {
	class VAO {
	public:
		VAO();

		void bind() const;

		void setAttribute(GLuint index, const Buffer& buffer, GLint size, GLenum type) const;

		void removeAttribute(GLuint index) const;

		~VAO();

	private:
		GLuint mVAO = 0;
	};
}