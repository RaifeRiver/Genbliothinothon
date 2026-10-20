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
       addPart(std::make_unique<part::Send>(this, glm::vec2{3.0f, 24.0f}, glm::vec2{244.0f, 20.0f}, "Audio"));

        mMathsBox = &addPart(std::make_unique<part::TextInput>(this, glm::vec2{3.0f, 47.0f}, glm::vec2{194.0f, 20.0f}));
        mMathsBox->setText("sin(3.14159*t*440/48000)");
    }

    std::string MathsWave::generateGLSL(const uint32_t index, std::unordered_map<std::string, std::string>& inputValues) {
       // Grab whatever text the user typed into the box
       std::string userMath = mMathsBox->getText();

       // Let's replace the word "samp" in your text box with "sampleI" so the shader understands it.
       size_t pos = 0;
       while ((pos = userMath.find('t', pos)) != std::string::npos) {
            userMath.replace(pos, 1, "sampleI/96000.0f");
            pos += 7; // Length of "sampleI" How do i make it t instead. i want to divide sampleI/96000
       }

       const std::string indexStr = std::to_string(index);

       // Construct the GLSL shader code
       // It sets up the audio float, begins the voice with your custom math, and ends it.
       std::string shaderCode = "float Audio0_" + indexStr + "=0.0f;\n";
       shaderCode += "VOICE_BEGIN(" + userMath + ")\n";
       shaderCode += "VOICE_END(Audio0_" + indexStr + ")";

       return shaderCode;
    }
}