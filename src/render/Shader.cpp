#include "Shader.h"

#include <fstream>
#include <sstream>
#include <vector>

namespace genbliothinothon::render {
	std::string loadShaderSource(const std::string& shaderPath) {
		std::ifstream shaderFile(shaderPath);
		if (!shaderFile.is_open()) {
			throw std::runtime_error("Failed to open shader file: " + shaderPath);
		}

		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();

		return shaderStream.str();
	}

	GLuint compileShader(const std::string& shaderSource, const GLenum shaderType) {
		const GLuint shader = glCreateShader(shaderType);
		const char* shaderCStr = shaderSource.c_str();
		glShaderSource(shader, 1, &shaderCStr, nullptr);
		glCompileShader(shader);

		int compileStatus;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
		if (!compileStatus) {
			int infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::vector<char> infoLog(infoLogLength);
			glGetShaderInfoLog(shader, infoLogLength, &infoLogLength, infoLog.data());
			throw std::runtime_error("Shader compilation failed: " + std::string(infoLog.begin(), infoLog.end()));
		}

		return shader;
	}

	Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
		const GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
		const GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, vertexShader);
		glAttachShader(mShaderProgram, fragmentShader);
		glLinkProgram(mShaderProgram);

		int linkStatus;
		glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &linkStatus);
		if (!linkStatus) {
			int infoLogLength;
			glGetProgramiv(mShaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::vector<char> infoLog(infoLogLength);
			glGetProgramInfoLog(mShaderProgram, infoLogLength, &infoLogLength, infoLog.data());
			throw std::runtime_error("Shader linking failed: " + std::string(infoLog.begin(), infoLog.end()));
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::bind() const {
		glUseProgram(mShaderProgram);
	}

	void Shader::setUniform(const std::string& name, const int32_t v) const {
		bind();
		glUniform1i(glGetUniformLocation(mShaderProgram, name.c_str()), v);
	}

	void Shader::setUniform(const std::string& name, const uint32_t v) const {
		bind();
		glUniform1ui(glGetUniformLocation(mShaderProgram, name.c_str()), v);
	}

	void Shader::setUniform(const std::string &name, const std::vector<float>& v) const {
		bind();
		glUniform1fv(glGetUniformLocation(mShaderProgram, name.c_str()), static_cast<GLint>(v.size()), v.data());
	}

	void Shader::setUniform(const std::string& name, const glm::uvec2 v) const {
		bind();
		glUniform2ui(glGetUniformLocation(mShaderProgram, name.c_str()), v.x, v.y);
	}

	Shader::~Shader() {
		glDeleteProgram(mShaderProgram);
	}

	std::unique_ptr<Shader> Shader::loadFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
		return std::make_unique<Shader>(loadShaderSource(vertexShaderPath), loadShaderSource(fragmentShaderPath));
	}
}
