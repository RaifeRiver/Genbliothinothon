#pragma once

#include <unordered_map>

#include "Module.h"
#include "part/TextInput.h"

namespace genbliothinothon::module {
	class MidiSequencer : public Module {
	public:
		explicit MidiSequencer(const glm::vec2& pos);

		std::string generateFunctions(uint32_t index) override;

		std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) override;

	private:
		part::TextInput* mTextInput = nullptr;
		float mLength = 0.0f;

		static uint32_t parseVariableInt(std::streamsize& pos, const uint8_t* buffer);
	};
}
