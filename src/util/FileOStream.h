#pragma once

#include <cstdint>
#include <fstream>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace genbliothinothon::util {
	class FileOStream {
	public:
		explicit FileOStream(const std::string& path);

		FileOStream& operator<<(uint32_t v);

		FileOStream& operator<<(float v);

		FileOStream& operator<<(bool v);

		FileOStream& operator<<(glm::vec2 v);

		FileOStream& operator<<(glm::vec4 v);

		FileOStream& operator<<(const std::string& v);

		~FileOStream();

	private:
		std::ofstream mStream;
	};
}
