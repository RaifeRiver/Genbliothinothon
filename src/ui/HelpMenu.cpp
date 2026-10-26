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
       auto list = std::make_unique<List>(std::vector<std::string>{"Basics", "Oscillator", "Modules One", "Modules Part Two"});
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
          case 2:
             loadModules();
             break;
          case 3:
             loadModules_Two();
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

       auto connectingSends = std::make_unique<Label>(
          "Click on the send, then click on the target. Dials are also valid\n"
          "targets for send, e.g. frequency on the oscillator."
       );
       mHelpContainer->addChild(std::move(connectingSends));

       auto outputHeader = std::make_unique<Label>("Output");
       outputHeader->setFontSize(18.0f);
       outputHeader->setPadding({0.0f, 10.0f, 0.0f, 5.0f});
       mHelpContainer->addChild(std::move(outputHeader));

       auto outputDescription = std::make_unique<Label>(
          "There is only ever one output module. It has left and right inputs."
       );
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

       auto waveTypes = std::make_unique<Label>(
          " - Sine: this wave generates a consistent and smooth form,\n"
          " it has one harmonic(itself) and you literally cannot have\n"
          " a smoother wave at the same amplitude, that is the definition.\n\n"

          "Saw: this wave type ramps up and then instantly drops down to -1 again.\n"
          " it is called saw because it looks like a saw.\n"
          " it has many harmonics and is great for bass, sounds like a\n"

          " buzzy and annoying but really loud and harsh fly\n\n"
          "Inv Saw: This is simply the opposite of a sawtooth wave,\n"
          " it is slightly different sound but interchangeable\n"
          " and can sound more this and that depending on configuration.\n\n"

          "Triangle: this sounds like a sine wave but it has some more harmonics.\n"
          " I would use it if you want a sin sound but need a bit more tang,\n"
          " it is a pretty nice and not too harsh wave form.\n"
          " But of course has infinite harmonics technically like the others,\n"
          " on this one the higher harmonics decrease in volume faster so\n"
          " generally it is just a slightly less plain sine wave\n\n"

          "Square: This one is rich, it is amazing for anything that needs tons\n"
          " of overtones. All quite loud. But if you want to use it a lot i\n"
          " recommend to cut out the high frequencies. This one is fun but i\n"
          " would usually use saw rather than it. It definitely has its place\n"
          " as the buzz king though. If you want that."
       );
       mHelpContainer->addChild(std::move(waveTypes));

       auto inputsHeader = std::make_unique<Label>("Inputs");
       inputsHeader->setFontSize(18.0f);
       inputsHeader->setPadding({0.0f, 10.0f, 0.0f, 3.0f});
       mHelpContainer->addChild(std::move(inputsHeader));

       auto inputs = std::make_unique<Label>(
          " - Tone - controls the pitch, if there is a send it will apply a\n"
          "   semitone offset to the inputted frequency.\n"
          "Amplitude - controls the volume, multiplied with the send input."
       );
       mHelpContainer->addChild(std::move(inputs));
    }

    void HelpMenu::loadModules() const {
       auto title = std::make_unique<Label>("Modules One");
       title->setFontSize(24.0f);
       title->setPadding({0.0f, 0.0f, 0.0f, 5.0f});
       mHelpContainer->addChild(std::move(title));

       auto waveTypesHeader = std::make_unique<Label>("Modules With Descriptions");
       waveTypesHeader->setFontSize(18.0f);
       waveTypesHeader->setPadding({0.0f, 10.0f, 0.0f, 3.0f});
       mHelpContainer->addChild(std::move(waveTypesHeader));

       auto waveTypes = std::make_unique<Label>(
          " - AHDSR:\n"
          "The AHDSR is an audio packet module. It has attack, hold, sustain,\n"
          "decay and release. These are labeled AT HT DT SP RT AC DC RC.\n"
          "Each of these stand for the corresponding variable, to expand\n"
          "these into words you can convert with T = time, C = curve\n"
          "(negative gets the centre of the line closer to y axis positive\n"
          "is further.) P = percent but you measure from 0.0 to 1.0 where\n"
          "the percentage is represented as a decimal value. Then A, H, D,\n"
          "S, R correspond to the 5 letters in the acronym in order I stated\n"
          "earlier. It is just a amplitude multiplier and can only be used\n"
          "with midi players I believe.\n\n"

          "CSV sequencer takes in a csv file and outputs audio as if it\n"
          "were midi, you must manually convert audio to csv so I do not\n"
          "recommend using this, also It is not perfectly implemented with\n"
          "all other features. ../res/songs/<the song file> is the file\n"
          "selection, you have only got song_1_voice1.csv and\n"
          "song_1_voice2.csv which are the same song just different parts.\n\n"

          "Maths wave, is slightly buggy but usually takes in a GLSL maths\n"
          "equation, (one line only for now) and outputs audio based on that,\n"
          "the example uses correct values. Base you equations on that if\n"
          "you wish to use this.\n\n"

          "MIDI sequencer: you must select a file by typing in\n"
          "../res/songs/<the song file> and at the moment there is only\n"
          "song_1.mid or song_2.mid but they should work nicely. The output\n"
          "and variables should explain themselves.\n\n"

          "Noise source: this generates random white noise with a low and\n"
          "high pass filter. Simple.\n\n"

          "Oscillator: this generates a wave type. Already explained in the\n"
          "help section. Well that says which waves there are and what they\n"
          "are like. Basically you just select a wave type and send midi into\n"
          "the amplitude knob or just send it straight to anything else. This\n"
          "one is the basis of the program. Fairly simple though. some would\n"
          "say this is the entire synthesiser."
       );
       mHelpContainer->addChild(std::move(waveTypes));
    }

    void HelpMenu::loadModules_Two() const {
       auto title = std::make_unique<Label>("Modules Part Two");
       title->setFontSize(24.0f);
       title->setPadding({0.0f, 0.0f, 0.0f, 5.0f});
       mHelpContainer->addChild(std::move(title));

       auto waveTypesHeader = std::make_unique<Label>("Modules With Descriptions");
       waveTypesHeader->setFontSize(18.0f);
       waveTypesHeader->setPadding({0.0f, 10.0f, 0.0f, 3.0f});
       mHelpContainer->addChild(std::move(waveTypesHeader));

       auto waveTypes = std::make_unique<Label>(
          "AntiDepressant: this is a pun on the name. It removes negativity.\n"
          "Literally; it removes the negative half of the wave only leaving\n"
          "the positive half. This generates a clip distortion and a lot\n"
          "of overtones.\n\n"

          "BitCrusher: this one is well known, it just takes the input\n"
          "amplitude and reduces the precision of it. If set to 1 bit it\n"
          "will make anything into a variable square wave. You can see this\n"
          "if you connect it to something that measures frequencies. The\n"
          "overtones match up. I personally like this one but it has its\n"
          "limits and is slightly niche in most types of music.\n\n"

          "Depresser: this is a pun on the AntiDepressant pun. It does\n"
          "the opposite, only the bottom half of the wave.\n\n"
          "FullRectifier: this one rectifies the signal, the bottom half\n"
          "is flipped upside down and smacked over the top half of the wave.\n"
          "It can make fun sounds and is over all quite interesting.\n\n"

          "HardClip: this one clips stuff. It amplifies the signal and\n"
          "cuts off anything above amplitude 1 and below amplitude -1.\n"
          "You can get some crazy distortion with this one but it also can\n"
          "be used to make warm tones if done right. I like it.\n\n"

          "OverInvert: this one pretty much just inverts the signal.\n"
          "That is it.\n\n"
          "SoftDistort: this is one of my favourites. As a number approaches\n"
          "full amplitude it is tapered off. with a gain of 1 you will never\n"
          "get signal above 1 or below -1, but you won't get clipy tons of\n"
          "overtones until you pump the drive way up to like 10. You can\n"
          "get some really warm bass with this.\n\n"

          "TapeSaturation: this replicates the saturation from an old tape\n"
          "machine and is a type of distortion. Very very good for warm tones\n"
          "or anything that needs overtones without being crazy like the\n"
          "hard clip.\n\n"

          "Tremlo: this is spelt wrong for a smaller name and makes the\n"
          "audio sway from channel to channel of its outputs. Very fun.\n\n"
          "TubeSaturation: this one is super fun. Replicates an old vacuum\n"
          "tube amp. At least a bit. I like it a lot. But I can't recommend\n"
          "what to use it for. Up to you!\n\n"
          "XOR xors the signal with another value. Self explanatory."
       );
       mHelpContainer->addChild(std::move(waveTypes));
    }
}