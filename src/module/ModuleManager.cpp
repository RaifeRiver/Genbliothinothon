#include "ModuleManager.h"

#include <algorithm>
#include <iostream>
#include <ranges>

#include "Genbliothinothon.h"
#include "Output.h"
#include "part/RemoveButton.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	ModuleManager::ModuleManager() {
		part::Part::initParts();

		mModules[mNextID++] = Module::createModule("Output", glm::vec2{500.0f, 100.0f});
	}

	void ModuleManager::addModule(std::unique_ptr<Module> module) {
		module->setID(mNextID);
		mModules[mNextID++] = std::move(module);
	}

	Module* ModuleManager::getModule(const uint32_t id) {
		return mModules[id].get();
	}

	void ModuleManager::removeModule(const uint32_t id) {
		const std::vector<std::unique_ptr<part::Part>>& parts = mModules[id]->getParts();
		for (uint32_t i = 0; i < parts.size(); i++) {
			if (const auto sendTarget = dynamic_cast<part::SendTarget*>(parts[i].get())) {
				for (const auto [moduleID, partID] : sendTarget->getInputs()) {
					auto* send = dynamic_cast<part::Send*>(mModules[moduleID]->getParts()[partID].get());
					send->setSendTarget({});
					send->setDirty(true);
				}
			}
			else if (const auto send = dynamic_cast<part::Send*>(parts[i].get())) {
				std::optional<part::PartID> target = send->getSendTarget();
				if (!target.has_value()) {
					continue;
				}
				auto [moduleID, partID] = target.value();
				auto* sendTarget2 = dynamic_cast<part::SendTarget*>(mModules[moduleID]->getParts()[partID].get());
				std::erase(sendTarget2->getInputs(), part::PartID{.module = id, .part = i});
			}
		}

		mModules.erase(id);

		std::cout << "Erasing module " << id << std::endl;
	}

	void ModuleManager::resetModules() {
		mModules.clear();
		mNextID = 0;
		mModules[mNextID++] = Module::createModule("Output", glm::vec2{500.0f, 100.0f});
	}

	void ModuleManager::update(Genbliothinothon& genbliothinothon) {
		bool dirty = false;
		for (const auto& module: mModules | std::views::values) {
			if (module->isDirty()) {
				module->clearDirty();
				dirty = true;
			}
		}

		if (dirty) {
			genbliothinothon.getAudioPlayer().rebuildShader(*this);
		}
	}

	void ModuleManager::save(util::FileOStream& stream) const {
		stream << mNextID;
		stream << static_cast<uint32_t>(mModules.size());
		for (const std::unique_ptr<Module>& module: mModules | std::views::values) {
			module->save(stream);
		}
	}

	void ModuleManager::load(const uint32_t version, util::FileIStream& stream) {
		mModules.clear();
		stream >> mNextID;
		uint32_t moduleCount;
		stream >> moduleCount;
		mModules.reserve(moduleCount);
		for (uint32_t i = 0; i < moduleCount; i++) {
			std::unique_ptr<Module> module = Module::loadModule(version, stream);
			mModules[module->getID()] = std::move(module);
		}
	}
}
