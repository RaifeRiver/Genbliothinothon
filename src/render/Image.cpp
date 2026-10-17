#include "Image.h"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace genbliothinothon::render {
	Image::Image(const std::string& path) {
		int width;
		int height;
		int channels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		if (!data) {
			throw std::runtime_error("Failed to load image: " + path);
		}

		glGenTextures(1, &mTexture);
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		mSize = {width, height};
	}

	Image::Image(const glm::uvec2 size, const GLenum format) : mSize(size) {
		glGenTextures(1, &mTexture);
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, static_cast<GLint>(size.x), static_cast<GLint>(size.y), 0, format == GL_RG32I ? GL_RG_INTEGER : GL_RGBA, format == GL_RG32I ? GL_INT : GL_FLOAT, nullptr);
	}

	void Image::bind(const uint32_t slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, mTexture);
	}

	Image::~Image() {
		glDeleteTextures(1, &mTexture);
	}
}
