#include "AHDSR.h"

#include <iostream>

#include "Genbliothinothon.h"
#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	AHDSR::AHDSR(const glm::vec2& pos) : Module(pos, {299.0f, 195.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{299.0f, 195.0f}, "AHDSR"));
		addPart(std::make_unique<part::RemoveButton>(this));
		// colum 1 row 2
		mVolume = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{3.0f, 47.0f}, glm::vec2{145.0f, 20.0f}, "Volume", "0.0f"));
		mVolume->setInputCallback([this] {
			inputCallback();
		});
		// colum 2 row 1
		mNoteStart = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{151.0f, 24.0f}, glm::vec2{145.0f, 20.0f}, "NoteStart", "0"));
		mNoteStart->setInputCallback([this] {
			inputCallback();
		});
		// colum 2 row 3
		mVolumeOut = &addPart(std::make_unique<part::Send>(this, glm::vec2{151.0f, 172.0f}, glm::vec2{145.0f, 20.0f}, "Volume", "float"));
		// colum 2 row 2
		mNoteHeld = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{151.0f, 47.0f}, glm::vec2{145.0f, 20.0f}, "NoteHeld", "false"));
		mNoteHeld->setCombiningMode(part::CombiningMode::OR);
		mNoteHeld->setInputCallback([this] {
			inputCallback();
		});
		// colum 1 row 1
		mFrequency = &addPart(std::make_unique<part::GenericSendTarget>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{145.0f, 20.0f}, "Frequency", "0.0f"));
		mFrequency->setInputCallback([this] {
			inputCallback();
		});
		// colum 1 row 3
		mFrequencyOut = &addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 172.0f}, glm::vec2{145.0f, 20.0f}, "Frequency", "float", 1));

		mAT = &addPart(std::make_unique<part::Dial>(this, glm::vec2{3.0f, 73.0f}, glm::vec2{50.0f, 50.0f}, "AT", 0.0f, 20000.0f, 50.0f));
		mAT->setInputCallback([this] {
			inputCallback();
		});
		mAC = &addPart(std::make_unique<part::Dial>(this, glm::vec2{38.0f, 108.0f}, glm::vec2{50.0f, 50.0f}, "AC", -50.0f, 50.0f, -10.0f));
		mAC->setInputCallback([this] {
			inputCallback();
		});
		mHT = &addPart(std::make_unique<part::Dial>(this, glm::vec2{73.0f, 73.0f}, glm::vec2{50.0f, 50.0f}, "HT", 0.0f, 20000.0f, 1.0f));
		mHT->setInputCallback([this] {
			inputCallback();
		});
		mSP = &addPart(std::make_unique<part::Dial>(this, glm::vec2{108.0f, 108.0f}, glm::vec2{50.0f, 50.0f}, "SP", 0.0f, 1.0f, 0.7f));
		mSP->setInputCallback([this] {
			inputCallback();
		});
		mDT = &addPart(std::make_unique<part::Dial>(this, glm::vec2{143.0f, 73.0f}, glm::vec2{50.0f, 50.0f}, "DT", 0.0f, 20000.0f, 500.0f));
		mDT->setInputCallback([this] {
			inputCallback();
		});
		mDC = &addPart(std::make_unique<part::Dial>(this, glm::vec2{178.0f, 108.0f}, glm::vec2{50.0f, 50.0f}, "DC", -50.0f, 50.0f, 4.0f));
		mDC->setInputCallback([this] {
			inputCallback();
		});
		mRT = &addPart(std::make_unique<part::Dial>(this, glm::vec2{213.0f, 73.0f}, glm::vec2{50.0f, 50.0f}, "RT", 0.0f, 20000.0f, 600.0f));
		mRT->setInputCallback([this] {
			inputCallback();
		});
		mRC = &addPart(std::make_unique<part::Dial>(this, glm::vec2{248.0f, 108.0f}, glm::vec2{50.0f, 50.0f}, "RC", -50.0f, 50.0f, -10.0f));
		mRC->setInputCallback([this] {
			inputCallback();
		});
	}

	AHDSR::AHDSR(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		const std::vector<std::unique_ptr<part::Part>>& parts = getParts();
		mVolume = reinterpret_cast<part::GenericSendTarget*>(parts[2].get());
		mVolume->setInputCallback([this] {
			inputCallback();
		});
		mNoteStart = reinterpret_cast<part::GenericSendTarget*>(parts[3].get());
		mNoteStart->setInputCallback([this] {
			inputCallback();
		});
		mVolumeOut = reinterpret_cast<part::Send*>(parts[4].get());
		mNoteHeld = reinterpret_cast<part::GenericSendTarget*>(parts[5].get());
		mNoteHeld->setInputCallback([this] {
			inputCallback();
		});
		mFrequency = reinterpret_cast<part::GenericSendTarget*>(parts[6].get());
		mFrequency->setInputCallback([this] {
			inputCallback();
		});
		mFrequencyOut = reinterpret_cast<part::Send*>(parts[7].get());
		mAT = reinterpret_cast<part::Dial*>(parts[8].get());
		mAT->setInputCallback([this] {
			inputCallback();
		});
		mAC = reinterpret_cast<part::Dial*>(parts[9].get());
		mAC->setInputCallback([this] {
			inputCallback();
		});
		mHT = reinterpret_cast<part::Dial*>(parts[10].get());
		mHT->setInputCallback([this] {
			inputCallback();
		});
		mSP = reinterpret_cast<part::Dial*>(parts[11].get());
		mSP->setInputCallback([this] {
			inputCallback();
		});
		mDT = reinterpret_cast<part::Dial*>(parts[12].get());
		mDT->setInputCallback([this] {
			inputCallback();
		});
		mDC = reinterpret_cast<part::Dial*>(parts[13].get());
		mDC->setInputCallback([this] {
			inputCallback();
		});
		mRT = reinterpret_cast<part::Dial*>(parts[14].get());
		mRT->setInputCallback([this] {
			inputCallback();
		});
		mRC = reinterpret_cast<part::Dial*>(parts[15].get());
		mRC->setInputCallback([this] {
			inputCallback();
		});
	}

	std::string AHDSR::generateGLSL(const std::string send) {
		return std::string("AHDSR($Volume,$Sample-$NoteStart,$AT,$AC,$HT,$DT,$DC,$SP,$RT,$RC,$NoteHeld,$Frequency).") + (send == "Volume" ? "x" : "y");
	}

	void AHDSR::inputCallback() {
		ModuleManager& moduleManager = Genbliothinothon::getInstance()->getModuleManager();
		uint32_t volumeChannelCount = 0;
		if (const std::optional<part::PartID>& volumeInput = mVolume->getInput()) {
			volumeChannelCount += reinterpret_cast<part::Send*>(moduleManager.getModule(volumeInput->module)->getParts()[volumeInput->part].get())->getChannelCount();
		}
		uint32_t frequencyChannelCount = 0;
		if (const std::optional<part::PartID>& frequencyInput = mFrequency->getInput()) {
			frequencyChannelCount += reinterpret_cast<part::Send*>(moduleManager.getModule(frequencyInput->module)->getParts()[frequencyInput->part].get())->getChannelCount();
		}
		mChannelCount = std::max(volumeChannelCount, frequencyChannelCount);
		mFrequencyOut->setChannelCount(mChannelCount);
		mVolumeOut->setChannelCount(mChannelCount);
		std::cout << "AHDSR channels: " << mChannelCount << std::endl;
	}
}
