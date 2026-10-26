#pragma once

#include "Component.h"

namespace genbliothinothon::ui {
	class ModuleArea : public Component {
	public:
		explicit ModuleArea(glm::vec2 size);

		[[nodiscard]] glm::vec2& getSize() {
			return mSize;
		}

		void setSize(glm::vec2 size);

		void onPress(glm::vec2 p) override;

		void onRelease() override;

		void onMouseMove(glm::vec2 pos) override;

		void render(glm::vec2 pos, render::UIRenderer& renderer) override;

		[[nodiscard]] glm::vec2 calculateSize(render::UIRenderer& renderer) override;

	private:
		glm::vec2 mSize;
	};
}
