#include "GenericSendTarget.h"

#include <utility>

namespace genbliothinothon::module::part {
	GenericSendTarget::GenericSendTarget(Module* parent, const glm::vec2 pos, const glm::vec2 size, const std::string& name, std::string defaultValue) : SendTarget(sType, parent, pos, size, name), mDefaultValue(std::move(defaultValue)) {}

	GenericSendTarget::GenericSendTarget(const uint32_t version, util::FileIStream& stream) : SendTarget(version, stream) {
		stream >> mDefaultValue;
	}

	void GenericSendTarget::save(util::FileOStream& stream) {
		SendTarget::save(stream);
		stream << mDefaultValue;
	}

	void GenericSendTarget::setType(const uint32_t type) {
		sType = type;
	}

	void GenericSendTarget::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rect(pos, size, glm::vec4{0.06f, 0.06f, 0.11f, 1.0f});
		const float textHeight = size.y / 2.0f - 7.0f;
		renderer.text(pos + glm::vec2{2.0f, textHeight}, 14.0f, getName(), glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, static_cast<int32_t>(size.x));
	}
}
