#pragma once

#include "Module.h"
#include "part/Dial.h"

namespace genbliothinothon::module {
    class Tremlo : public Module {
    public:
        explicit Tremlo(const glm::vec2& pos);

        Tremlo(uint32_t version, util::FileIStream& stream);

        std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) override;

    private:
        part::Dial* mAmplitude;
        part::Dial* mPeriod;
        part::Dial* mGain;
        part::Dial* mSmooth;
    };
}
