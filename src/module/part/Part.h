#pragma once

#include "render/RenderContext.h"

namespace genbliothinothon::module::part {
	class Part {
	public:
		explicit Part(glm::vec2 pos, glm::vec2 size);

		void render(glm::vec2 pos, render::RenderContext& context);

		[[nodiscard]] glm::vec2 getPos() const {
			return mPos;
		}

		[[nodiscard]] glm::vec2 getSize() const {
			return mSize;
		}

		virtual void click(glm::vec2 pos);

		virtual void clickOff(glm::vec2 pos);

		virtual ~Part() = default;

	protected:
		virtual void render_(glm::vec2 pos, render::RenderContext& context) = 0;

	private:
		glm::vec2 mPos;
		glm::vec2 mSize;
	};
}
