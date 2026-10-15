#pragma once

#include "render/RenderContext.h"

namespace genbliothinothon {
	class Genbliothinothon {
	public:
		Genbliothinothon();

		void run() const;

	private:
		render::RenderContext mRenderContext;
	};
}
