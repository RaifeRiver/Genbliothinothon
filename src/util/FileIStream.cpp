#include "FileIStream.h"

namespace genbliothinothon::util {
	FileIStream::FileIStream(const std::string& path) : mStream(path, std::ios::binary) {
		if (!mStream.is_open()) {
			throw std::runtime_error("Failed to open file: " + path);
		}
	}

	FileIStream& FileIStream::operator>>(uint32_t& v) {
		mStream.read(reinterpret_cast<char*>(&v), sizeof(v));
		return *this;
	}

	FileIStream& FileIStream::operator>>(float& v) {
		mStream.read(reinterpret_cast<char*>(&v), sizeof(v));
		return *this;
	}

	FileIStream & FileIStream::operator>>(bool &v) {
		mStream.read(reinterpret_cast<char*>(&v), sizeof(v));
		return *this;
	}

	FileIStream& FileIStream::operator>>(glm::vec2& v) {
		mStream.read(reinterpret_cast<char*>(&v), sizeof(v));
		return *this;
	}

	FileIStream& FileIStream::operator>>(glm::vec4& v) {
		mStream.read(reinterpret_cast<char*>(&v), sizeof(v));
		return *this;
	}

	FileIStream & FileIStream::operator>>(std::string& v) {
		uint32_t size;
		*this >> size;
		v.resize(size);
		mStream.read(v.data(), size);
		return *this;
	}

	FileIStream::~FileIStream() {
		mStream.close();
	}
}
