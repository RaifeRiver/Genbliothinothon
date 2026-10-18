#pragma once

#include "Module.h"

namespace genbliothinothon::module {
	class Output : public Module {
	public:
		explicit Output(const glm::vec2& pos);

		std::string generateGLSL(std::unordered_map<std::string, std::string>& inputValues) override;
	};
}
