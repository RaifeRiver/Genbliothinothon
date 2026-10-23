#pragma once

#include "part/Dial.h"
#include "Module.h"
#include "part/GenericSendTarget.h"

namespace genbliothinothon::module {
	class AHDSR : public Module {
	public:
		explicit AHDSR(const glm::vec2& pos);

		AHDSR(uint32_t version, util::FileIStream& stream);

		std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) override;

	private:
		part::GenericSendTarget* mVolume;
		part::GenericSendTarget* mNoteStart;
		part::GenericSendTarget* mNoteHeld;
		part::GenericSendTarget* mFrequency;
		part::Dial* mAT;
		part::Dial* mAC;
		part::Dial* mHT;
		part::Dial* mSP;
		part::Dial* mDT;
		part::Dial* mDC;
		part::Dial* mRT;
		part::Dial* mRC;

	};
}
