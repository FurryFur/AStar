//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : 
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#include <nanogui\nanogui.h>

#include "App.h"

int main() {
	nanogui::init();

	nanogui::ref<AStarApp> app = new AStarApp;
	app->setVisible(true);

	nanogui::mainloop();

	nanogui::shutdown();
	exit(EXIT_SUCCESS);
}