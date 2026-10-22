#pragma once

#include <cstdint>
#include <fstream>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace genbliothinothon::util {
	class FileIStream {
	public:
		explicit FileIStream(const std::string& path);

		FileIStream& operator>>(uint32_t& v);

		FileIStream& operator>>(float& v);

		FileIStream& operator>>(bool& v);

		FileIStream& operator>>(glm::vec2& v);

		FileIStream& operator>>(glm::vec4& v);

		FileIStream& operator>>(std::string& v);

		~FileIStream();

	private:
		std::ifstream mStream;
	};
}
