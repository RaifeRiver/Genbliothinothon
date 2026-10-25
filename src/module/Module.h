#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "part/Part.h"
#include "util/FileIStream.h"
#include "util/FileOStream.h"

namespace genbliothinothon::module {
	class Module {
	public:
		Module(glm::vec2 pos, glm::vec2 size);

		Module(uint32_t version, util::FileIStream& stream);

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

		void setDragged(const bool dragged);

		[[nodiscard]] bool isDirty() const;

		void clearDirty();

		void setDirty();

		[[nodiscard]] uint32_t getID() const {
			return mID;
		}

		void setID(uint32_t id);

		[[nodiscard]] uint32_t getType() const {
			return mType;
		}

		void drag(glm::vec2 distance, glm::vec2 pos);

		void endDrag() const;

		virtual std::string generateFunctions(uint32_t index);

		virtual std::string generateGLSL(uint32_t index, std::unordered_map<std::string, std::string>& inputValues) = 0;

		virtual void save(util::FileOStream& stream) const;

		void render(glm::vec2 pos, render::RenderContext& context) const;

		virtual ~Module() = default;

		static std::unique_ptr<Module> createModule(const std::string& type, glm::vec2 pos);

		static std::unique_ptr<Module> loadModule(uint32_t version, util::FileIStream& stream);

		[[nodiscard]] static std::vector<std::tuple<std::string, std::function<std::unique_ptr<Module>(glm::vec2)>, std::function<std::unique_ptr<Module>(uint32_t, util::FileIStream&)>>>& getModules() {
			initModules();
			return sModuleRegistry;
		}

	private:
		static inline std::vector<std::tuple<std::string, std::function<std::unique_ptr<Module>(glm::vec2)>, std::function<std::unique_ptr<Module>(uint32_t, util::FileIStream&)>>> sModuleRegistry;

		std::vector<std::unique_ptr<part::Part>> mParts;
		glm::vec2 mPos = {0.0f, 0.0f};
		glm::vec2 mSize = {0.0f, 0.0f};
		bool mDragged = false;
		bool mDirty = false;
		uint32_t mID = 0;
		uint32_t mType = 0;

		template <std::derived_from<Module> T> static void registerModule(std::string name) {
			uint32_t type = sModuleRegistry.size();
			sModuleRegistry.emplace_back(name, [type](glm::vec2 pos) -> std::unique_ptr<Module> {
				std::unique_ptr<Module> module = std::make_unique<T>(pos);
				module->mType = type;
				return module;
			}, [type](uint32_t version, util::FileIStream& stream) -> std::unique_ptr<Module> {
				std::unique_ptr<Module> module = std::make_unique<T>(version, stream);
				module->mType = type;
				return module;
			});
		}

		static void initModules();

		static uint32_t registerModules();
	};
}
