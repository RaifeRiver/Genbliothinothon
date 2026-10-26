#pragma once

#include "Module.h"
#include "part/TextInput.h"

namespace genbliothinothon::module {
    class MathsWave : public Module {
    public:
        explicit MathsWave(const glm::vec2& pos);

    	MathsWave(uint32_t version, util::FileIStream& stream);

        std::string generateGLSL(std::string send) override;

    private:
        part::TextInput* mMathsBox;
    };
}