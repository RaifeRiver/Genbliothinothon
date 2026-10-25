#pragma once

#include <memory>

#include "Module.h"
#include "util/FileIStream.h"
#include "util/FileOStream.h"

namespace genbliothinothon {
	class Genbliothinothon;
}

namespace genbliothinothon::module {
	class ModuleManager {
	public:
		ModuleManager();

		void addModule(std::unique_ptr<Module> module);

		[[nodiscard]] Module* getModule(uint32_t id);

		void removeModule(uint32_t id);

		[[nodiscard]] std::unordered_map<uint32_t, std::unique_ptr<Module>>& getModules() {
			return mModules;
		}

		void resetModules();

		void update(Genbliothinothon& genbliothinothon);

		void save(util::FileOStream& stream) const;

		void load(uint32_t version, util::FileIStream& stream);

	private:
		uint32_t mNextID = 0;
		std::unordered_map<uint32_t, std::unique_ptr<Module>> mModules;
	};
}
