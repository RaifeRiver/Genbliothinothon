#pragma once

#include "Module.h"
#include "part/Dial.h"

namespace genbliothinothon::module {
    class Tremlo : public Module {
    public:
        explicit Tremlo(const glm::vec2& pos);

        Tremlo(uint32_t version, util::FileIStream& stream);

        std::string generateGLSL(std::string send) override;

    private:
        part::Dial* mAmplitude;
        part::Dial* mPeriod;
        part::Dial* mGain;
        part::Dial* mSmooth;
    };
}
