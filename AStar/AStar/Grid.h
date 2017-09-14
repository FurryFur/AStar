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
	
	// Set the node at the specified grid position
	void setGridNode(size_t row, size_t col, nanogui::ref<Node> node);

	// Get the node at the specified grid position
	nanogui::ref<Node> getGridNode(size_t row, size_t col) const;

	// Weird proxy for safe 2D index operator
	class IndexProxy {
	public:
		IndexProxy(const std::array<nanogui::ref<Node>, s_kGridSize>* _array);

		// Access the node in the 2D grid through an index proxy
		// Returns a null reference on invalid index
		nanogui::ref<Node> operator[](size_t col) const;
	private:
		const std::array<nanogui::ref<Node>, s_kGridSize>* m_array;
	};

	// Access the node in the 2D grid through an index proxy
	// Returns an index proxy to null references on invalid index
	const IndexProxy operator[](size_t row) const;

private:
	NDArray<nanogui::ref<Node>, s_kGridSize, s_kGridSize> m_internalGrid;
};

