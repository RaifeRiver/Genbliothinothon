#pragma once

#include "Module.h"
#include "part/Dial.h"

namespace genbliothinothon::module {
    class HardClip : public Module {
    public:
        explicit HardClip(const glm::vec2& pos);

        HardClip(uint32_t version, util::FileIStream& stream);

        std::string generateGLSL(std::string send) override;

    private:
        part::Dial* mAmplitude;
        part::Dial* mDrive;
        part::Dial* mGain;
    };
}
