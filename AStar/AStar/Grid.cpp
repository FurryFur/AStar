#include "Grid.h"
#include "Node.h"

Grid::Grid()
{
}


Grid::~Grid()
{
}

void Grid::setGridNode(size_t row, size_t col, nanogui::ref<Node> node)
{
	m_internalGrid[row][col] = std::move(node);
}

nanogui::ref<Node> Grid::getGridNode(size_t row, size_t col)
{
	return m_internalGrid[row][col];
}