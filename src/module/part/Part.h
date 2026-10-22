#pragma once

#include "render/RenderContext.h"
#include "util/FileIStream.h"
#include "util/FileOStream.h"

namespace genbliothinothon::module {
	class Module;
}

namespace genbliothinothon::module::part {
	struct PartID {
		uint32_t module;
		uint32_t part;
	};

	bool operator==(PartID a, PartID b);

	class Part {
	public:
		Part(uint32_t type, Module* parent, glm::vec2 pos, glm::vec2 size);

		Part(uint32_t version, util::FileIStream& stream);

		void render(glm::vec2 pos, render::RenderContext& context);

		[[nodiscard]] Module* getParent() const {
			return mParent;
		}

		void setParent(Module* parent);

		[[nodiscard]] glm::vec2 getPos() const {
			return mPos;
		}

		[[nodiscard]] glm::vec2 getSize() const {
			return mSize;
		}

		[[nodiscard]] bool isDirty() const {
			return mDirty;
		}

		void setDirty(const bool dirty) {
			mDirty = dirty;
		}

		[[nodiscard]] bool isDragged() const {
			return mDragged;
		}

		void setDragged(const bool dragged) {
			mDragged = dragged;
		}

		[[nodiscard]] uint32_t getType() const {
			return mType;
		}

		void setType(uint32_t type);

		virtual void click(glm::vec2 pos);

		virtual void clickOff(glm::vec2 pos);

		virtual bool drag(glm::vec2 distance);

		virtual void endDrag();

		virtual void charPressed(char key);

		virtual void keyPressed(int key);

		virtual void save(util::FileOStream& stream);

		virtual ~Part() = default;

		static std::unique_ptr<Part> loadPart(uint32_t version, util::FileIStream& stream);

		static void initParts();

	protected:
		virtual void render_(glm::vec2 pos, render::RenderContext& context) = 0;

	private:
		static inline std::vector<std::function<std::unique_ptr<Part>(uint32_t, util::FileIStream&)>> sPartRegistry;

		Module* mParent = nullptr;
		glm::vec2 mPos = {};
		glm::vec2 mSize = {};
		bool mDirty = false;
		bool mDragged = false;
		uint32_t mType = 0;

		template <std::derived_from<Part> T> static void registerPart() {
			uint32_t type = sPartRegistry.size();
			T::setType(type);
			sPartRegistry.push_back([type](uint32_t version, util::FileIStream& stream) -> std::unique_ptr<Part> {
				std::unique_ptr<Part> part = std::make_unique<T>(version, stream);
				part->mType = type;
				return part;
			});
		}

		static uint32_t registerParts();
	};
}
