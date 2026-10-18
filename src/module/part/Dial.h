#pragma once

#include "SendTarget.h"

namespace genbliothinothon::module::part {
	class Dial : public SendTarget {
	public:
		Dial(Module* parent, glm::vec2 pos, glm::vec2 size, const std::string& name, float min, float max);

		Dial(Module* parent, glm::vec2 pos, glm::vec2 size, const std::string& name, float min, float max, float initial);

		std::string getDefaultValueString() override;

		bool drag(glm::vec2 distance) override;

		void endDrag() override;

		[[nodiscard]] float getValue() const {
			return mValue;
		}

		void setValue(float value);

		[[nodiscard]] float getMin() const {
			return mMin;
		}

		void setMin(float min);

		[[nodiscard]] float getMax() const {
			return mMax;
		}

		void setMax(float max);

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		float mMin;
		float mMax;
		float mValue;
	};
}
