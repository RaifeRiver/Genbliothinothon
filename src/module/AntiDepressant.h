#pragma once

#include "Module.h"
#include "part/Dial.h"

namespace genbliothinothon::module {
    class AntiDepressant : public Module {
    public:
        explicit AntiDepressant(const glm::vec2& pos);

        AntiDepressant(uint32_t version, util::FileIStream& stream);

        std::string generateGLSL(std::string send) override;

    private:
        part::Dial* mAmplitude;
        part::Dial* mDrive;
        part::Dial* mGain;
    };
}
