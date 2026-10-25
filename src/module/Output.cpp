#include "Output.h"

#include "Genbliothinothon.h"
#include "ModuleManager.h"
#include "part/Background.h"
#include "part/Send.h"

namespace genbliothinothon::module {
	Output::Output(const glm::vec2& pos) : Module(pos, glm::vec2{130.0f, 80.0f}) {
		addPart(std::make_unique<part::Background>(this, glm::vec2{0.0f, 0.0f}, glm::vec2{130.0f, 80.0f}, "Output"));
		left = &addPart(std::make_unique<part::Output>(this, glm::vec2{5.0f, 25.0f}, glm::vec2{50.0f, 50.0f}, "Left"));
		left->setCombiningMode(part::CombiningMode::SUM);
		right = &addPart(std::make_unique<part::Output>(this, glm::vec2{75.0f, 25.0f}, glm::vec2{50.0f, 50.0f}, "Right"));
		right->setCombiningMode(part::CombiningMode::SUM);
	}

	Output::Output(const uint32_t version, util::FileIStream& stream) : Module(version, stream) {
		const std::vector<std::unique_ptr<part::Part>>& parts = getParts();
		left = reinterpret_cast<part::Output*>(parts[1].get());
		right = reinterpret_cast<part::Output*>(parts[2].get());
	}

	std::string Output::generateGLSL(uint32_t, std::unordered_map<std::string, std::string>& inputValues) {
		uint32_t channelCount = 0;
		ModuleManager& moduleManager = Genbliothinothon::getInstance()->getModuleManager();
		for (const auto [module, part]: left->getInputs()) {
			channelCount = std::max(channelCount, reinterpret_cast<part::Send*>(moduleManager.getModule(module)->getParts()[part].get())->getChannelCount());
		}
		for (const auto [module, part]: right->getInputs()) {
			channelCount = std::max(channelCount, reinterpret_cast<part::Send*>(moduleManager.getModule(module)->getParts()[part].get())->getChannelCount());
		}
		std::string leftGLSL;
		std::string rightGLSL;
		for (uint32_t channel = 0; channel < channelCount; channel++) {
			const auto leftIt = inputValues.find("Left" + std::to_string(channel) + "_");
			const auto rightIt = inputValues.find("Right" + std::to_string(channel) + "_");
			if (leftIt != inputValues.end()) {
				leftGLSL += leftIt->second + "+";
			}
			if (rightIt != inputValues.end()) {
				rightGLSL += rightIt->second + "+";
			}
		}
		if (leftGLSL.empty()) {
			leftGLSL = "0.0f";
		}
		else {
			leftGLSL.pop_back();
		}
		if (rightGLSL.empty()) {
			rightGLSL = "0.0f";
		}
		else {
			rightGLSL.pop_back();
		}
		return "float leftAudio=" + leftGLSL + ";\nfloat rightAudio=" + rightGLSL + ";\n";
	}
}
