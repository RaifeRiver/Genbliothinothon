#include "FileOStream.h"

namespace genbliothinothon::util {
	FileOStream::FileOStream(const std::string& path) : mStream(path, std::ios::binary) {
		if (!mStream.is_open()) {
			throw std::runtime_error("Failed to open file: " + path);
		}
	}

	FileOStream& FileOStream::operator<<(const uint32_t v) {
		mStream.write(reinterpret_cast<const char*>(&v), sizeof(v));
		return *this;
	}

	FileOStream& FileOStream::operator<<(const float v) {
		mStream.write(reinterpret_cast<const char*>(&v), sizeof(v));
		return *this;
	}

	FileOStream & FileOStream::operator<<(const bool v) {
		mStream.write(reinterpret_cast<const char*>(&v), sizeof(v));
		return *this;
	}

	FileOStream& FileOStream::operator<<(const glm::vec2 v) {
		mStream.write(reinterpret_cast<const char*>(&v), sizeof(v));
		return *this;
	}

	FileOStream& FileOStream::operator<<(const glm::vec4 v) {
		mStream.write(reinterpret_cast<const char*>(&v), sizeof(v));
		return *this;
	}

	FileOStream & FileOStream::operator<<(const std::string& v) {
		const auto size = static_cast<uint32_t>(v.size());
		*this << size;
		mStream.write(v.data(), size);
		return *this;
	}

	FileOStream::~FileOStream() {
		mStream.close();
	}
}
