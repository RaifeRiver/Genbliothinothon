#pragma once

#include "SendTarget.h"

namespace genbliothinothon::module::part {
	class Output : public SendTarget {
	public:
		Output(Module* parent, const glm::vec2& pos, const glm::vec2& size, const std::string& name);

		Output(uint32_t version, util::FileIStream& stream);

		std::string getDefaultValueString() override;

		static void setType(uint32_t type);

		[[nodiscard]] static uint32_t getType() {
			return sType;
		}

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		static inline uint32_t sType;
	};
}
