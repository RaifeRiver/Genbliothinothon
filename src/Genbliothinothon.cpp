#include "Genbliothinothon.h"

namespace genbliothinothon {
	Genbliothinothon::Genbliothinothon() {

	}

	void Genbliothinothon::run() {
		while (!mRenderContext.shouldClose()) {
			mRenderContext.clearColour(0.05f, 0.05f, 0.1f);

			render::UIRenderer& uiRenderer = mRenderContext.getUIRenderer();

			mAudioPlayer.update(mRenderContext);

			uiRenderer.text({100.0f, 100.0f}, 100.0f, "Hello World!", {1.0f, 1.0f, 1.0f, 1.0f});
			uiRenderer.rect({100.0f, 200.0f}, {100.0f, 100.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, "button");

			uiRenderer.render(mRenderContext);
			mRenderContext.swapBuffers();
		}
	}
}
