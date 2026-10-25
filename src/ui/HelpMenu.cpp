#include "HelpMenu.h"

#include <memory>
#include <vector>

#include "Button.h"
#include "Genbliothinothon.h"
#include "Label.h"
#include "List.h"
#include "SideLayoutManager.h"
#include "SimpleLayoutManager.h"

namespace genbliothinothon::ui {
	HelpMenu::HelpMenu() : Container({800.0f, 800.0f}) {
		setLayoutManager(std::make_unique<SideLayoutManager>());
		setPadding({5.0f, 5.0f, 5.0f, 5.0f});

		auto listContainer = std::make_unique<Container>(glm::vec2{0.0f, 790.0f});
		listContainer->setLayoutManager(std::make_unique<SimpleLayoutManager>(Direction::VERTICAL));
		listContainer->setBackgroundColour({0.05f, 0.05f, 0.1f, 1.0f});
		auto list = std::make_unique<List>(std::vector<std::string>{"Basics", "Oscillator"});
		list->setBackgroundColour({0.05f, 0.05f, 0.1f, 1.0f});
		list->setSelectCallback([this](const uint32_t id) {
			loadHelp(id);
		});
		float listSizeX = list->calculateSize(Genbliothinothon::getInstance()->getRenderContext().getUIRenderer()).x;
		listContainer->setSize({listSizeX, 790.0f});
		listContainer->addChild(std::move(list), new SideLayoutData{Side::TOP_LEFT});
		addChild(std::move(listContainer), new SideLayoutData{Side::LEFT});

		auto helpContainer = std::make_unique<Container>(glm::vec2{785.0f - listSizeX, 790.0f});
		helpContainer->setLayoutManager(std::make_unique<SimpleLayoutManager>(Direction::VERTICAL));
		helpContainer->setBackgroundColour({0.05f, 0.05f, 0.1f, 1.0f});
		helpContainer->setPadding({3.0f, 3.0f, 3.0f, 3.0f});
		mHelpContainer = &addChild(std::move(helpContainer), new SideLayoutData{Side::RIGHT});

		auto closeButton = std::make_unique<Button>("Close");
		closeButton->setCallback([this] {
			remove();
		});
		addChild(std::move(closeButton), new SideLayoutData{Side::BOTTOM_RIGHT});

		loadBasics();
	}

	void HelpMenu::loadHelp(const uint32_t id) const {
		mHelpContainer->getChildren().clear();
		switch (id) {
			case 0:
				loadBasics();
				break;
			case 1:
				loadOscillator();
				break;
			default:
				throw std::runtime_error("Invalid help id");
		}
	}

	void HelpMenu::loadBasics() const {
		auto title = std::make_unique<Label>("Basics");
		title->setFontSize(24.0f);
		title->setPadding({0.0f, 0.0f, 0.0f, 3.0f});
		mHelpContainer->addChild(std::move(title));

		auto connectingSendsHeader = std::make_unique<Label>("Connecting sends");
		connectingSendsHeader->setFontSize(18.0f);
		connectingSendsHeader->setPadding({0.0f, 0.0f, 0.0f, 5.0f});
		mHelpContainer->addChild(std::move(connectingSendsHeader));

		auto connectingSends = std::make_unique<Label>("Click on the send, then click on the target. Dials are also valid targets for\nsend, e.g. frequency on the oscillator.");
		mHelpContainer->addChild(std::move(connectingSends));

		auto outputHeader = std::make_unique<Label>("Output");
		outputHeader->setFontSize(18.0f);
		outputHeader->setPadding({0.0f, 10.0f, 0.0f, 5.0f});
		mHelpContainer->addChild(std::move(outputHeader));

		auto outputDescription = std::make_unique<Label>("There is only ever one output module. It has left and right inputs.");
		mHelpContainer->addChild(std::move(outputDescription));
	}

	void HelpMenu::loadOscillator() const {
		auto title = std::make_unique<Label>("Oscillator Module");
		title->setFontSize(24.0f);
		title->setPadding({0.0f, 0.0f, 0.0f, 5.0f});
		mHelpContainer->addChild(std::move(title));

		auto description = std::make_unique<Label>("Generates a wave form.");
		mHelpContainer->addChild(std::move(description));

		auto waveTypesHeader = std::make_unique<Label>("Wave Types");
		waveTypesHeader->setFontSize(18.0f);
		waveTypesHeader->setPadding({0.0f, 10.0f, 0.0f, 3.0f});
		mHelpContainer->addChild(std::move(waveTypesHeader));

		auto waveTypes = std::make_unique<Label>(" - Sine: this wave generates a consistent and smooth form, it has one harmonic(itself)\n and you literally cannot have a smoother wave at the same amplitude, that is the definition.\n\n - Saw: this wave type ramps up and then instantly drops down to -1 again.\n it is called saw because it looks like a saw.\n it has many harmonics and is great for bass, sounds like a \nbuzzy and annoying but really loud and harsh fly\n\n - Inv Saw: This is simply the opposite of a sawtooth wave,\n it is slightly different sound but interchangeable\n and can sound more this and that depending on configuration.\n\n - Triangle: this sounds like a sine wave but it has some more harmonics.\n I would use it if you want a sin sound but need a bit more tang,\n it is a pretty nice and not too harsh wave form. \nBut of course has infinite harmonics technically like the others,\n on this one the higher harmonics decrease in volume faster so generally it is just a slightly less plain sine wave\n\n - Square: This one is rich, it is amazing for anything that needs tons of overtones.\n All quite loud. But if you want to use it a lot i recommend to cut out the high frequencies.\n This one is fun but i would usually use saw rather than it.\n It definitely has its place as the buzz king though. If you want that.");
		mHelpContainer->addChild(std::move(waveTypes));

		auto inputsHeader = std::make_unique<Label>("Inputs");
		inputsHeader->setFontSize(18.0f);
		inputsHeader->setPadding({0.0f, 10.0f, 0.0f, 3.0f});
		mHelpContainer->addChild(std::move(inputsHeader));

		auto inputs = std::make_unique<Label>(" - Tone - controls the pitch, if there is a send it will apply a semitone offset\n   to the inputted frequency.\n - Amplitude - controls the volume, multiplied with the send input.");
		mHelpContainer->addChild(std::move(inputs));
	}
}
