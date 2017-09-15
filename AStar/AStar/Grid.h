#pragma once

#include <array>
#include <nanogui\object.h>

#include "Utils.h"

class Node;

class Grid
{
public:
	static const size_t s_kGridSize = 16;

	Grid();
	~Grid();
	
	// Sets the node at the specified grid position.
	void setGridNode(size_t row, size_t col, nanogui::ref<Node> node);

	// Gets the node at the specified grid position.
	nanogui::ref<Node> getGridNode(size_t row, size_t col) const;

	// Weird proxy for safe 2D index operations.
	// Stores a pointer to a grid row that can then be indexed by column.
	class IndexProxy {
	public:
		IndexProxy(const std::array<nanogui::ref<Node>, s_kGridSize>* _array);

		// Accesses a node in the 2D grid through an index proxy.
		// Returns a null reference on invalid index.
		nanogui::ref<Node> operator[](size_t col) const;
	private:
		const std::array<nanogui::ref<Node>, s_kGridSize>* m_array;
	};

	// Accesses a node in the 2D grid through an index proxy.
	// Returns an index proxy pointing to the specified row in the grid.
	// If an invalid row is supplied, a dummy index proxy is returned that 
	// always returns a null reference when indexed into.
	const IndexProxy operator[](size_t row) const;

	// Returns whether two nodes are connectable.
	bool areConnectable(nanogui::ref<Node> from, nanogui::ref<Node> to) const;

private:
	NDArray<nanogui::ref<Node>, s_kGridSize, s_kGridSize> m_internalGrid;
	nanogui::ref<Node> m_startNode;
	nanogui::ref<Node> m_endNode;
};

