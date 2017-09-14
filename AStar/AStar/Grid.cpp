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

nanogui::ref<Node> Grid::getGridNode(size_t row, size_t col) const
{
	if (row < s_kGridSize && col < s_kGridSize)
		return m_internalGrid[row][col];

	return nanogui::ref<Node>{};
}

const Grid::IndexProxy Grid::operator[](size_t row) const
{
	if (row < m_internalGrid.size())
		return Grid::IndexProxy(&m_internalGrid[row]);

	return Grid::IndexProxy(nullptr);
}

Grid::IndexProxy::IndexProxy(const std::array<nanogui::ref<Node>, s_kGridSize>* _array) : 
	m_array(_array) 
{ 

}

nanogui::ref<Node> Grid::IndexProxy::operator[](size_t col) const
{
	if (m_array && col < m_array->size())
		return (*m_array)[col];

	return nanogui::ref<Node>{};
}
