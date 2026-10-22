#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class Background : public Part {
	public:
		Background(Module* parent, glm::vec2 pos, glm::vec2 size, std::string name);

		Background(uint32_t version, util::FileIStream& stream);

		[[nodiscard]] const std::string& getName() const {
			return mName;
		}

		void setName(const std::string& name);

		void save(util::FileOStream& stream) override;

		static void setType(uint32_t type);

		[[nodiscard]] static uint32_t getType() {
			return sType;
		}

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		static inline uint32_t sType;
		std::string mName;
	};
}
