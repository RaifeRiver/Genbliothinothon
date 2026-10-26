#include "MathsWave.h"
#include <memory>

#include "part/Background.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
    MathsWave::MathsWave(const glm::vec2& pos) : Module(pos, glm::vec2{250.0f, 100.0f}) {
       // Draw the background
       addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{250.0f, 100.0f}, "Math Maths"));
       addPart(std::make_unique<part::RemoveButton>(this));

       // Audio Output
       addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{244.0f, 20.0f}, "Audio", "float"));

        mMathsBox = &addPart(std::make_unique<part::TextInput>(this, glm::vec2{3.0f, 47.0f}, glm::vec2{194.0f, 20.0f}));
        mMathsBox->setText("sin(3.1415926535897932384626*t*2*440)");
    }

    MathsWave::MathsWave(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
	    mMathsBox = reinterpret_cast<part::TextInput*>(getParts()[3].get());
    }

    std::string MathsWave::generateGLSL(std::string) {
       // Grab whatever text the user typed into the box
       std::string userMath = mMathsBox->getText();

       // Let's replace the word "samp" in your text box with "sampleI" so the shader understands it.
       size_t pos = 0;
       while ((pos = userMath.find('t', pos)) != std::string::npos) {
            userMath.replace(pos, 1, "$Sample/96000.0f");
            pos += 16; // Length of "sampleI" How do i make it t instead. i want to divide sampleI/96000
       }

       return userMath;
    }
}