#pragma once

#include "Part.h"

namespace genbliothinothon::module::part {
	class Send;

	class SendTarget : public Part {
	public:
		SendTarget(Module* parent, glm::vec2 pos, const glm::vec2& size, std::string name);

		[[nodiscard]] std::vector<Send*>& getInputs() {
			return mInputs;
		}

		void addInput(Send* input);

		virtual std::string getDefaultValueString() = 0;

		[[nodiscard]] std::string getName() const {
			return mName;
		}

	private:
		std::vector<Send*> mInputs;
		std::string mName;
	};
}
