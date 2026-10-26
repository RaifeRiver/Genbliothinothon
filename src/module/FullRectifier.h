#pragma once

#include "Module.h"
#include "part/Dial.h"

namespace genbliothinothon::module {
    class FullRectifier : public Module {
    public:
        explicit FullRectifier(const glm::vec2& pos);

        FullRectifier(uint32_t version, util::FileIStream& stream);

        std::string generateGLSL(std::string send) override;

    private:
        part::Dial* mAmplitude;
        part::Dial* mDrive;
        part::Dial* mGain;
    };
}
