#include "Background.h"

#include <algorithm>
#include <utility>

#include "module/Module.h"

namespace genbliothinothon::module::part {
	Background::Background(Module* parent, const glm::vec2 pos, const glm::vec2 size, std::string name) : Part(sType, parent, pos, size), mName(std::move(name)) {}

	Background::Background(const uint32_t version, util::FileIStream& stream) : Part(version, stream) {
		stream >> mName;
	}

	void Background::setName(const std::string& name) {
		mName = name;
	}

	void Background::save(util::FileOStream& stream) {
		Part::save(stream);
		stream << mName;
	}

	void Background::setType(const uint32_t type) {
		sType = type;
	}

	void Background::render_(const glm::vec2 pos, render::RenderContext& context) {
		render::UIRenderer& renderer = context.getUIRenderer();
		const glm::vec2 size = getSize();
		renderer.rect(pos, size, glm::vec4{0.08f, 0.08f, 0.14f, 1.0f}, "module_background");
		renderer.text(pos + glm::vec2{4.0f, 2.0f}, 18.0f, mName, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
	}
}
