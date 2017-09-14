#pragma once

#include <nanogui/nanogui.h>

using namespace nanogui;

class Cell : public Widget {
public:
	Cell(Window* window);

	// Draw the cell
	virtual void draw(NVGcontext* ctx) override;

	// Handle clicking on the cell
	virtual bool mouseButtonEvent(const Vector2i& p, int button, bool down, int modifiers) override;

	// Handle entering the cell with a mouse button down
	virtual bool mouseEnterEvent(const Vector2i& p, bool enter) override;

private:
	bool m_toggle;
};