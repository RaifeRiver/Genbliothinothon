#include "Genbliothinothon.h"

#include "ui/Button.h"
#include "ui/HelpMenu.h"
#include "ui/HoverMenu.h"
#include "ui/SideLayoutManager.h"
#include "ui/SimpleLayoutManager.h"

namespace genbliothinothon {
	Genbliothinothon::Genbliothinothon() : mUIContainer(mRenderContext.getWindowSize()) {
		sInstance = this;

		try {
			util::FileIStream stream("../save.bin");
			uint32_t version;
			stream >> version;
			mModuleManager.load(version, stream);
			mAudioPlayer.rebuildShader(mModuleManager);
		}
		catch (std::exception&) {}

		mUIContainer.setLayoutManager(std::make_unique<ui::SideLayoutManager>());
		mUIContainer.setBackgroundColour(glm::vec4{0.0f, 0.0f, 0.0f, 0.0f});

		auto menuBar = std::make_unique<ui::Container>(glm::vec2{-1.0f, 20.0f});
		menuBar->setLayoutManager(std::make_unique<ui::SimpleLayoutManager>(ui::Direction::HORIZONTAL));

		auto fileMenu = std::make_unique<ui::HoverMenu>("File");
		auto fileMenuContainer = std::make_unique<ui::Container>(glm::vec2{50.0f, 36.0f});
		fileMenuContainer->setLayoutManager(std::make_unique<ui::SimpleLayoutManager>(ui::Direction::VERTICAL));

		auto newButton = std::make_unique<ui::Button>("New");
		newButton->setCallback([this] {
			mModuleManager.resetModules();
			mAudioPlayer.rebuildShader(mModuleManager);
			mCameraPos = {0.0f, 0.0f};
		});
		fileMenuContainer->addChild(std::move(newButton));

		auto exitButton = std::make_unique<ui::Button>("Exit");
		exitButton->setCallback([this] {
			mRunning = false;
		});
		fileMenuContainer->addChild(std::move(exitButton));

		fileMenu->setMenu(std::move(fileMenuContainer));
		menuBar->addChild(std::move(fileMenu));

		auto helpMenu = std::make_unique<ui::HoverMenu>("Help");
		auto helpMenuContainer = std::make_unique<ui::Container>(glm::vec2{50.0f, 18.0f});
		helpMenuContainer->setLayoutManager(std::make_unique<ui::SimpleLayoutManager>(ui::Direction::VERTICAL));

		auto helpButton = std::make_unique<ui::Button>("Help");
		helpButton->setCallback([this] {
			mUIContainer.addChild(std::make_unique<ui::HelpMenu>(), new ui::SideLayoutData{.side = ui::Side::CENTRE});
		});
		helpMenuContainer->addChild(std::move(helpButton));

		helpMenu->setMenu(std::move(helpMenuContainer));
		menuBar->addChild(std::move(helpMenu));

		mUIContainer.addChild(std::move(menuBar), new ui::SideLayoutData{.side = ui::Side::TOP});
	}

	void Genbliothinothon::run() {
		while (!mRenderContext.shouldClose() && mRunning) {
			mRenderContext.clearColour(0.05f, 0.05f, 0.1f);

			mModuleManager.update(*this);
			mUIContainer.render(glm::vec2{0.0f, 0.0f}, mRenderContext.getUIRenderer());
			mAudioPlayer.update(mRenderContext);
			mRenderContext.getUIRenderer().render(mRenderContext);
			mRenderContext.swapBuffers();
		}
	}

	Genbliothinothon::~Genbliothinothon() {
		util::FileOStream stream("../save.bin");
		stream << 1u;
		mModuleManager.save(stream);
	}
}
