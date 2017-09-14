#pragma once

#include <list>
#include <memory>

#include <nanogui/nanogui.h>

#include "Utils.h"

class Grid;

class Node : public nanogui::Widget {
public:
	Node(nanogui::Window* window, size_t row, size_t col, Grid& grid);

	// Draw the cell
	virtual void draw(NVGcontext* ctx) override;

	// Handle clicking on the cell
	virtual bool mouseButtonEvent(const nanogui::Vector2i& p, int button, bool down, int modifiers) override;

	// Handle entering the cell with a mouse button down
	virtual bool mouseEnterEvent(const nanogui::Vector2i& p, bool enter) override;

	// Add pathing connection to the specified node
	void addConnection(nanogui::ref<Node> node);

	static const size_t s_kGridSize = 16;
private:

	// Remove pathing connection to specified node
	std::list<nanogui::ref<Node>>::iterator removeConnection(nanogui::ref<Node> node);

	void obstructionEvent(bool obstructed);

	bool m_obstructed;
	std::list<nanogui::ref<Node>> m_connections;
	Grid& m_grid;
};