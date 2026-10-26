#pragma once

#include "part/Dial.h"
#include "Module.h"
#include "part/GenericSendTarget.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	class AHDSR : public Module {
	public:
		explicit AHDSR(const glm::vec2& pos);

		AHDSR(uint32_t version, util::FileIStream& stream);

		std::string generateGLSL(std::string send) override;

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
		part::Send* mVolumeOut;
		part::Send* mFrequencyOut;
		uint32_t mChannelCount = 0;

		void inputCallback();
	};
}
