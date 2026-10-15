#include "Genbliothinothon.h"

namespace genbliothinothon {
	Genbliothinothon::Genbliothinothon() {

	}

	void Genbliothinothon::run() {
		while (!mRenderContext.shouldClose()) {
			mRenderContext.clearColour(0.05f, 0.05f, 0.1f);

			mRenderContext.swapBuffers();
		}
	}
}
