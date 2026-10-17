#pragma once

#include "Module.h"

namespace genbliothinothon::module {
	class Output : public Module {
	public:
		explicit Output(const glm::vec2& pos);

		std::string generateGLSL(const std::string& inputName) override;
	};
}
