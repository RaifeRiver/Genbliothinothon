#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "part/Part.h"

namespace genbliothinothon::module {
	class Module {
	public:
		explicit Module(glm::vec2 pos, glm::vec2 size);

		template <std::derived_from<part::Part> T> T& addPart(std::unique_ptr<T> part) {
			mParts.push_back(std::move(part));
			return static_cast<T&>(*mParts.back());
		}

		[[nodiscard]] std::vector<std::unique_ptr<part::Part>>& getParts() {
			return mParts;
		}

		[[nodiscard]] glm::vec2 getPos() const {
			return mPos;
		}

		void setPos(glm::vec2 pos);

		[[nodiscard]] glm::vec2 getSize() const {
			return mSize;
		}

		void setSize(glm::vec2 size);

		[[nodiscard]] bool isDragged() const {
			return mDragged;
		}

		void setDragged(const bool dragged) {
			mDragged = dragged;
		}

		[[nodiscard]] bool isDirty() const;

		void clearDirty() const;

		void drag(glm::vec2 distance);

		void endDrag() const;

		virtual std::string generateFunctions(uint32_t index);

		virtual std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) = 0;

		void render(glm::vec2 pos, render::RenderContext& context) const;

		virtual ~Module() = default;

	private:
		std::vector<std::unique_ptr<part::Part>> mParts;
		glm::vec2 mPos;
		glm::vec2 mSize;
		bool mDragged = false;
	};
}
