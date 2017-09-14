#pragma once

#include <nanogui\object.h>

#include "Utils.h"

class Node;

class Grid
{
public:
	Grid();
	~Grid();
	
	// Set the node at the specified grid position
	void setGridNode(size_t row, size_t col, nanogui::ref<Node> node);

	// Get the node at the specified grid position
	nanogui::ref<Node> getGridNode(size_t row, size_t col);

	static const size_t s_kGridSize = 16;

private:
	NDArray<nanogui::ref<Node>, s_kGridSize, s_kGridSize> m_internalGrid;
};

