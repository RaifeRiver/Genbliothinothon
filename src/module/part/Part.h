#pragma once

#include "render/RenderContext.h"

namespace genbliothinothon::module::part {
	class Part {
	public:
		explicit Part(glm::vec2 pos);

		void render(glm::vec2 pos, render::RenderContext& context);

		[[nodiscard]] glm::vec2 getPos() const {
			return mPos;
		}

		virtual ~Part() = default;

	protected:
		virtual void render_(glm::vec2 pos, render::RenderContext& context) = 0;

	private:
		glm::vec2 mPos;
	};
}
