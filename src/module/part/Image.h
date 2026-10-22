#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class Image : public Part {
	public:
		Image(Module* parent, glm::vec2 pos, glm::vec2 size, std::string texture, glm::vec4 tint = {1.0f, 1.0f, 1.0f, 1.0f});

		Image(uint32_t version, util::FileIStream& stream);

		void save(util::FileOStream& stream) override;

		static void setType(uint32_t type);

		[[nodiscard]] static uint32_t getType() {
			return sType;
		}

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		static inline uint32_t sType;
		std::string mTexture;
		glm::vec4 mTint = {1.0f, 1.0f, 1.0f, 1.0f};
	};
}
