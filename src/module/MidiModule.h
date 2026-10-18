#pragma once

#include "Module.h"
#include "part/TextInput.h"

namespace genbliothinothon::module {
	class MidiModule : public Module {
	public:
		explicit MidiModule(glm::vec2 pos);

		std::string generateFunctions(uint32_t index) override;

		std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) override;

		[[nodiscard]] part::TextInput* getTextInput() const {
			return mTextInput;
		}

	private:
		part::TextInput* mTextInput = nullptr;
		float mLength = 0.0f;
	};
}
