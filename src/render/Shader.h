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

		void setUniform(const std::string& name, int32_t v) const;

		void setUniform(const std::string& name, uint32_t v) const;

		~Shader();

	private:
		GLuint mShaderProgram;
	};
}