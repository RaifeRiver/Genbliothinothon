#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	enum class CombiningMode : uint32_t {
		AVERAGE = 0,
		SUM = 1,
		OR = 2
	};

	class SendTarget : public Part {
	public:
		SendTarget(uint32_t type, Module* parent, glm::vec2 pos, const glm::vec2& size, std::string name, bool multiChannel = false);

		SendTarget(uint32_t version, util::FileIStream& stream);

		[[nodiscard]] std::vector<PartID>& getInputs() {
			return mInputs;
		}

		void addInput(PartID input);

		[[nodiscard]] bool isMultiChannel() const {
			return mMultiChannel;
		}
		
		void setMultiChannel(bool multiChannel);

		[[nodiscard]] CombiningMode getCombiningMode() const {
			return mCombiningMode;
		}

		void setCombiningMode(CombiningMode combiningMode);

		virtual std::string getDefaultValueString() = 0;

		[[nodiscard]] std::string getName() const {
			return mName;
		}

		void save(util::FileOStream& stream) override;

	private:
		std::vector<PartID> mInputs;
		std::string mName;
		bool mMultiChannel = false;
		CombiningMode mCombiningMode = CombiningMode::AVERAGE;
	};
}
