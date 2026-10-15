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
			glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog.data());
			throw std::runtime_error("Shader compilation failed: " + std::string(infoLog.begin(), infoLog.end()));
		}

		return shader;
	}

	Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
		const std::string vertexShaderSource = loadShaderSource(vertexShaderPath);
		const GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
		const std::string fragmentShaderSource = loadShaderSource(fragmentShaderPath);
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
			glGetProgramInfoLog(mShaderProgram, infoLogLength, nullptr, infoLog.data());
			throw std::runtime_error("Shader linking failed: " + std::string(infoLog.begin(), infoLog.end()));
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader::~Shader() {
		glDeleteProgram(mShaderProgram);
	}
}
