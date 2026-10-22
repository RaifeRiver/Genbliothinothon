#pragma once

#include "SendTarget.h"

namespace genbliothinothon::module::part {
	class GenericSendTarget : public SendTarget {
	public:
		GenericSendTarget(Module* parent, glm::vec2 pos, glm::vec2 size, const std::string& name, std::string defaultValue, bool multiChannel = false);

		GenericSendTarget(uint32_t version, util::FileIStream& stream);

		[[nodiscard]] std::string getDefaultValueString() override {
			return mDefaultValue;
		}

		void save(util::FileOStream& stream) override;

		static void setType(uint32_t type);

		[[nodiscard]] static uint32_t getType() {
			return sType;
		}

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		static inline uint32_t sType;
		std::string mDefaultValue;
	};
}
