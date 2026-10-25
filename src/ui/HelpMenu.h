#pragma once

#include "Container.h"

namespace genbliothinothon::ui {
	class HelpMenu : public Container {
	public:
		HelpMenu();

	private:
		Container* mHelpContainer;

		void loadHelp(uint32_t id) const;

		void loadBasics() const;

		void loadOscillator() const;
	};
}
