#include "Genbliothinothon.h"

namespace genbliothinothon {
	Genbliothinothon::Genbliothinothon() {
		sInstance = this;

		try {
			util::FileIStream stream("save.bin");
			uint32_t version;
			stream >> version;
			mModuleManager.load(version, stream);
		}
		catch (std::exception&) {}
	}

	void Genbliothinothon::run() {
		while (!mRenderContext.shouldClose()) {
			mRenderContext.clearColour(0.05f, 0.05f, 0.1f);

			mModuleManager.update(*this);
			mAudioPlayer.update(mRenderContext);
			mRenderContext.getUIRenderer().render(mRenderContext);
			mRenderContext.swapBuffers();
		}
	}

	Genbliothinothon::~Genbliothinothon() {
		util::FileOStream stream("save.bin");
		stream << 0u;
		mModuleManager.save(stream);
	}
}
