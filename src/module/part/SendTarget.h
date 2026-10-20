#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class Send;

	class SendTarget : public Part {
	public:
		SendTarget(Module* parent, glm::vec2 pos, const glm::vec2& size, std::string name, bool multiChannel = false);

		[[nodiscard]] std::vector<Send*>& getInputs() {
			return mInputs;
		}

		void addInput(Send* input);

		[[nodiscard]] bool isMultiChannel() const {
			return mMultiChannel;
		}
		
		void setMultiChannel(bool multiChannel);

		[[nodiscard]] bool isAveraged() const {
			return mAveraged;
		}

		void setAveraged(bool averaged);

		virtual std::string getDefaultValueString() = 0;

		[[nodiscard]] std::string getName() const {
			return mName;
		}

	private:
		std::vector<Send*> mInputs;
		std::string mName;
		bool mMultiChannel;
		bool mAveraged = true;
	};
}
