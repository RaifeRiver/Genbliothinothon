#pragma once

#include <optional>

#include "Part.h"

namespace genbliothinothon::module::part {
	enum class CombiningMode : uint32_t {
		AVERAGE = 0,
		SUM = 1,
		OR = 2
	};

	class SendTarget : public Part {
	public:
		SendTarget(uint32_t type, Module* parent, glm::vec2 pos, const glm::vec2& size, std::string name);

		SendTarget(uint32_t version, util::FileIStream& stream);

		[[nodiscard]] std::optional<PartID>& getInput() {
			return mInput;
		}

		SendTarget& setInput(std::optional<PartID> input);

		[[nodiscard]] CombiningMode getCombiningMode() const {
			return mCombiningMode;
		}

		SendTarget& setCombiningMode(CombiningMode combiningMode);

		[[nodiscard]] std::optional<std::function<void()>>& getInputCallback() {
			return mInputCallback;
		}

		SendTarget& setInputCallback(const std::optional<std::function<void()>>& callback);

		virtual std::string getDefaultValueString() = 0;

		[[nodiscard]] std::string getName() const {
			return mName;
		}

		void updateInputs() const;

		void save(util::FileOStream& stream) override;

	private:
		std::optional<PartID> mInput;
		std::string mName;
		CombiningMode mCombiningMode = CombiningMode::AVERAGE;
		std::optional<std::function<void()>> mInputCallback;
	};
}
