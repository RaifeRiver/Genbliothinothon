#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class Background : public Part {
	public:
		Background(glm::vec2 pos, glm::vec2 size, std::string name);

		[[nodiscard]] const std::string& getName() const {
			return mName;
		}

		void setName(const std::string& name);

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		std::string mName;
	};
}
