#pragma once

#include "Module.h"
#include "part/Dial.h"

namespace genbliothinothon::module {
    class BitCrusher : public Module {
    public:
        explicit BitCrusher(const glm::vec2& pos);

        BitCrusher(uint32_t version, util::FileIStream& stream);

        std::string generateGLSL(std::string send) override;

    private:
        part::Dial* mAmplitude;
        part::Dial* mBits;
        part::Dial* mGain;
    };
}
