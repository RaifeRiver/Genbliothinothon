#include "Genbliothinothon.h"

namespace genbliothinothon {
	Genbliothinothon::Genbliothinothon() {

	}

	void Genbliothinothon::run() {
		while (!mRenderContext.shouldClose()) {
			mRenderContext.clearColour(0.05f, 0.05f, 0.1f);

			render::TextRenderer& textRenderer = mRenderContext.getTextRenderer();

			mAudioPlayer.update(mRenderContext);

			textRenderer.text({100.0f, 100.0f}, 20.0f, "Hello World", {0.0f, 0.0f, 1.0f, 1.0f});

			textRenderer.render(mRenderContext);
			mRenderContext.swapBuffers();
		}
	}
}
