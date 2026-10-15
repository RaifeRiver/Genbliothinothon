#include "Genbliothinothon.h"

namespace genbliothinothon {
	Genbliothinothon::Genbliothinothon() {

	}

	void Genbliothinothon::run() const {
		while (!mRenderContext.shouldClose()) {
			mRenderContext.swapBuffers();
		}
	}
}
