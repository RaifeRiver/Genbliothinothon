#pragma once

#include <memory>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "glm/vec2.hpp"

namespace genbliothinothon::render {
	std::string loadShaderSource(const std::string& shaderPath);

	GLuint compileShader(const std::string& shaderSource, GLenum shaderType);

	class Shader {
	public:
		Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

		void bind() const;

		void setUniform(const std::string& name, int32_t v) const;

		void setUniform(const std::string& name, uint32_t v) const;

		void setUniform(const std::string& name, const std::vector<float>& v) const;

		void setUniform(const std::string& name, glm::uvec2 v) const;

		~Shader();

		static std::unique_ptr<Shader> loadFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	private:
		GLuint mShaderProgram;
	};
}