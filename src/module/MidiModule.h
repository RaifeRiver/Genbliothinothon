#pragma once

#include "Module.h"

namespace genbliothinothon::module {
	class MidiModule : public Module {
	public:
		explicit MidiModule(glm::vec2 pos);

		[[nodiscard]] std::string getInputCSV() const {
			return mInputCSV;
		}

		void setInputCSV(const std::string& inputCSV);

		std::string generateFunctions(uint32_t index) override;

		std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) override;

	private:
		std::string mInputCSV;
	};
}
