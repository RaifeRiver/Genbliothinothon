#pragma once

#include "SendTarget.h"

namespace genbliothinothon::module::part {
	class Dial : public SendTarget {
	public:
		Dial(Module* parent, glm::vec2 pos, glm::vec2 size, const std::string& name, float min, float max, bool multiChannel = false);

		Dial(Module* parent, glm::vec2 pos, glm::vec2 size, const std::string& name, float min, float max, float initial, bool multiChannel = false);

		Dial(uint32_t version, util::FileIStream& stream);

		std::string getDefaultValueString() override;

		void drag(glm::vec2 distance) override;

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

		void save(util::FileOStream& stream) override;

		static void setType(uint32_t type);

		[[nodiscard]] static uint32_t getType() {
			return sType;
		}

	protected:
		void render_(glm::vec2 pos, render::RenderContext& context) override;

	private:
		static inline uint32_t sType;
		float mMin = 0.0f;
		float mMax = 0.0f;
		float mValue = 0.0f;
	};
}
