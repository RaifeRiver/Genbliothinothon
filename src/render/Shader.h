#pragma once

#include <string>

#include "glad/glad.h"

namespace genbliothinothon::render {
	std::string loadShaderSource(const std::string& shaderPath);

	GLuint compileShader(const std::string& shaderSource, GLenum shaderType);

	class Shader {
	public:
		Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		void bind() const;

		~Shader();

	private:
		GLuint mShaderProgram;
	};
}