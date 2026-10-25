#pragma once

#include "Module.h"
#include "part/Output.h"

namespace genbliothinothon::module {
	class Output : public Module {
	public:
		explicit Output(const glm::vec2& pos);

		Output(uint32_t version, util::FileIStream& stream);

		std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) override;

	private:
		part::Output* left;
		part::Output* right;
	};
}
