#include "Oscillator.h"

#include <iostream>

#include "Genbliothinothon.h"
#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	Oscillator::Oscillator(const glm::vec2 pos) : Module(pos, glm::vec2{200.0f, 140.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{200.0f, 140.0f}, "Oscillator"));
		addPart(std::make_unique<part::RemoveButton>(this));
		mAudio = &addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 46.0f}, glm::vec2{194.0f, 20.0f}, "Amplitude", "float", 1));
		mFrequency = &addPart(std::make_unique<part::NoteDial>(this, glm::vec2{30.0f, 74.0f}, glm::vec2{50.0f, 50.0f}, "Tone"));
		mFrequency->setInputCallback([this] {
			inputCallback();
		});
		mVolume = &addPart(std::make_unique<part::Dial>(this, glm::vec2{110.0f, 74.0f}, glm::vec2{50.0f, 50.0f}, "Volume", 0.0f, 1.0f, 1.0f));
		mVolume->setInputCallback([this] {
			inputCallback();
		});
		mWaveType = &addPart(std::make_unique<part::Dropdown>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{194.0f, 20.0f}, std::vector<std::string>{"Sine", "Saw", "Inv Saw", "Triangle", "Square"}));
	}

	Oscillator::Oscillator(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		mAudio = reinterpret_cast<part::Send*>(getParts()[2].get());
		mFrequency = reinterpret_cast<part::NoteDial*>(getParts()[3].get());
		mFrequency->setInputCallback([this] {
			inputCallback();
		});
		mVolume = reinterpret_cast<part::Dial*>(getParts()[4].get());
		mVolume->setInputCallback([this] {
			inputCallback();
		});
		mWaveType = reinterpret_cast<part::Dropdown*>(getParts()[5].get());
	}

	std::string Oscillator::generateGLSL(std::string) {
		std::string functionName;
		switch (mWaveType->getSelectedOption()) {
			case 0:
				functionName = "sine";
				break;
			case 1:
				functionName = "saw";
				break;
			case 2:
				functionName = "Isaw";
				break;
			case 3:
				functionName = "tri";
				break;
			case 4:
				functionName = "sq";
				break;
			default:
				throw std::runtime_error("Invalid wave type");
		}
		return functionName + "($Sample,$Tone,$Volume)";
	}

	void Oscillator::inputCallback() {
		ModuleManager& moduleManager = Genbliothinothon::getInstance()->getModuleManager();
		mChannelCount = 0;
		if (const std::optional<part::PartID>& frequencyInput = mFrequency->getInput()) {
			mChannelCount += reinterpret_cast<part::Send*>(moduleManager.getModule(frequencyInput->module)->getParts()[frequencyInput->part].get())->getChannelCount();
		}
		mAudio->setChannelCount(mChannelCount);
		std::cout << "Oscillator channels: " << mChannelCount << std::endl;
	}
}
