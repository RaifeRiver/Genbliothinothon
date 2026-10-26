#pragma once

#include "Module.h"
#include "part/Output.h"

namespace genbliothinothon::module {
	class Output : public Module {
	public:
		explicit Output(const glm::vec2& pos);

		Output(uint32_t version, util::FileIStream& stream);

		std::string generateGLSL(std::string send) override;

	private:
		part::Output* left;
		part::Output* right;
	};
}
