#pragma once

#include "Component.h"
#include "LayoutManager.h"

namespace genbliothinothon::ui {
	class Container : public Component {
	public:
		explicit Container(glm::vec2 size);

		[[nodiscard]] LayoutManager* getLayoutManager() const {
			return mLayoutManager.get();
		}

		Container& setLayoutManager(std::unique_ptr<LayoutManager> layoutManager);

		[[nodiscard]] glm::vec4& getBackgroundColour() {
			return mBackgroundColour;
		}

		Container& setBackgroundColour(glm::vec4 backgroundColour);

		[[nodiscard]] glm::vec4& getPadding() {
			return mPadding;
		}

		Container& setPadding(glm::vec4 padding);

		[[nodiscard]] glm::vec2& getComponentPadding() {
			return mComponentPadding;
		}

		Container& setComponentPadding(glm::vec2 componentPadding);

		[[nodiscard]] std::vector<std::pair<std::unique_ptr<Component>, void*>>& getChildren() {
			return mChildren;
		}

		template <typename T> T& addChild(std::unique_ptr<T> child, void* data = nullptr) {
			mChildren.emplace_back(std::move(child), data);
			return reinterpret_cast<T&>(*mChildren.back().first);
		}

		void setSize(glm::vec2 size);

		void onHoverStop() override;

		void onPress(glm::vec2 pos) override;

		void onRelease() override;

		void onMouseMove(glm::vec2 pos) override;

		void render(glm::vec2 pos, render::UIRenderer& renderer) override;

		[[nodiscard]] glm::vec2 calculateSize(render::UIRenderer& renderer) override;

		[[nodiscard]] glm::vec4 calculateInteractionSize(render::UIRenderer& renderer) override;

	protected:
		std::unique_ptr<LayoutManager> mLayoutManager;
		glm::vec4 mBackgroundColour = {0.08f, 0.08f, 0.14f, 1.0f};
		glm::vec4 mPadding = {0.0f, 0.0f, 0.0f, 0.0f};
		glm::vec2 mComponentPadding = {3.0f, 3.0f};
		glm::vec2 mSize;
		std::vector<std::pair<std::unique_ptr<Component>, void*>> mChildren;
	};
}
