#pragma once

#include "Module.h"
#include "part/Send.h"
#include "part/TextInput.h"

namespace genbliothinothon::module {
	constexpr float RELEASE_TIME = 0.0f;

	class MidiSequencer : public Module {
	public:
		explicit MidiSequencer(const glm::vec2& pos);

		MidiSequencer(uint32_t version, util::FileIStream& stream);

		std::string generateFunctions(uint32_t index) override;

		std::string generateGLSL(std::string send) override;

	private:
		part::TextInput* mTextInput = nullptr;
		part::Send* mFrequency = nullptr;
		part::Send* mVolume = nullptr;
		part::Send* mNoteStart = nullptr;
		part::Send* mNoteHeld = nullptr;
		float mLength = -1.0f;
		uint32_t mChannelCount = 0;
		std::string mFunctions;

		void setChannelCounts() const;

		void loadMidiFile(const std::string& path);

		static uint32_t parseVariableInt(std::streamsize& pos, const uint8_t* buffer);
	};
}
