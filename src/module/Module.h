#pragma once

#include <memory>
#include <vector>

#include "part/Part.h"

namespace genbliothinothon::module {
	class Module {
	public:
		explicit Module(glm::vec2 pos);

		part::Part& addPart(std::unique_ptr<part::Part> part);

		[[nodiscard]] std::vector<std::unique_ptr<part::Part>>& getParts() {
			return mParts;
		}

		[[nodiscard]] glm::vec2 getPos() const {
			return mPos;
		}

		void setPos(glm::vec2 pos);

		void render(glm::vec2 pos, render::RenderContext& context) const;

		virtual ~Module() = default;

	private:
		std::vector<std::unique_ptr<part::Part>> mParts;
		glm::vec2 mPos;
	};
}
