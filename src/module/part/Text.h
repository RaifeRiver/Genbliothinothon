#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class Text : public Part {
	public:
		Text(Module* parent, glm::vec2 pos, float size, std::string text, glm::vec4 colour = {1.0f, 1.0f, 1.0f, 1.0f});

		Text(uint32_t version, util::FileIStream& stream);

		void save(util::FileOStream& stream) override;

		static void setType(uint32_t type);

		[[nodiscard]] static uint32_t getType() {
			return sType;
		}

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		static inline uint32_t sType;
		float mSize = 14.0f;
		std::string mText;
		glm::vec4 mColour = {1.0f, 1.0f, 1.0f, 1.0f};
	};
}
