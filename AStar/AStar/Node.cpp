#include <algorithm>

#include "Node.h"
#include "Grid.h"

using namespace nanogui;

Node::Node(Window * window, Grid& grid, size_t row, size_t col)
	: Widget(window)
	, m_grid{ grid }
	, m_row{ row }
	, m_col{ col }
	, m_obstructed{ false } 
{
	grid.setGridNode(row, col, this);
}

void Node::draw(NVGcontext * ctx)
{
	nvgSave(ctx);

	nvgBeginPath(ctx);
	nvgRect(ctx, static_cast<float>(mPos.x()), static_cast<float>(mPos.y()), static_cast<float>(mSize.x()), static_cast<float>(mSize.y()));
	if (m_obstructed) {
		nvgFillColor(ctx, nvgRGBA(255, 0, 0, 255));
		nvgFill(ctx);
	} else {
		nvgStrokeWidth(ctx, 5.0f);
		nvgStrokeColor(ctx, nvgRGBA(255, 0, 0, 255));
		nvgStroke(ctx);
	}

	
	for (ref<Node> node : m_connections) {
		nvgStrokeWidth(ctx, 5.0f);
		nvgStrokeColor(ctx, nvgRGBA(0, 255, 0, 160));
		nvgBeginPath(ctx);
		// Move to center of node
		float startX = mPos.x() + mSize.x() / 2.0f;
		float startY = mPos.y() + mSize.y() / 2.0f;
		nvgMoveTo(ctx, startX, startY);
		// Draw to center of connected node
		float endX = node->mPos.x() + node->mSize.x() / 2.0f;
		float endY = node->mPos.y() + node->mSize.y() / 2.0f;
		nvgLineTo(ctx, endX, endY);
		nvgStroke(ctx);
	}

	nvgRestore(ctx);

	Widget::draw(ctx);
}

bool Node::mouseButtonEvent(const Vector2i & p, int button, bool down, int modifiers)
{
	if (button == GLFW_MOUSE_BUTTON_1 && down) {
		if (!m_obstructed)
			obstructionEvent(true);

		return true;
	}
	if (button == GLFW_MOUSE_BUTTON_2 && down) {
		if (m_obstructed)
			obstructionEvent(false);

		return true;
	}

	return Widget::mouseButtonEvent(p, button, down, modifiers);
}

bool Node::mouseEnterEvent(const Vector2i & p, bool enter)
{
	Widget::mouseEnterEvent(p, enter);

	if (enter && glfwGetMouseButton(this->screen()->glfwWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		if (!m_obstructed)
			obstructionEvent(true);

		return true;
	}
	if (enter && glfwGetMouseButton(this->screen()->glfwWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		if (m_obstructed)
			obstructionEvent(false);

		return true;
	}

	return false;
}

bool Node::connect(ref<Node> node)
{
	if (!node)
		return false;

	// Add connection to node if one doesn't already exist
	if (std::find(m_connections.begin(), m_connections.end(), node) == m_connections.end()) {
		node->m_connections.push_back(this); // From that to this
		m_connections.push_back(std::move(node)); // From this to that
		return true;
	}

	return false;
}

size_t Node::getRow() const
{
	return m_row;
}

size_t Node::getCol() const
{
	return m_col;
}

bool Node::isObstructed() const
{
	return m_obstructed;
}

bool Node::connect(nanogui::ref<Node> node1, nanogui::ref<Node> node2)
{
	if (!node1 || !node2)
		return false;

	return node1->connect(node2);
}

bool Node::removeConnection(nanogui::ref<Node> node)
{
	if (!node)
		return false;

	// Find and remove connection
	auto it = m_connections.begin();
	while (it != m_connections.end()) {
		if (*it == node) {
			if (removeConnection(it) != m_connections.end())
				return true;
			else
				return false;
		} else {
			++it;
		}
	}

	return false;
}

bool Node::removeConnection(nanogui::ref<Node> node1, nanogui::ref<Node> node2)
{
	if (!node1 || !node2)
		return false;

	return node1->removeConnection(node2);
}

std::list<ref<Node>>::iterator Node::removeConnection(std::list<nanogui::ref<Node>>::iterator nodeIt)
{
	// Remove connection from specified node to this node
	for (auto it = (*nodeIt)->m_connections.begin(); it != (*nodeIt)->m_connections.end(); ++it) {
		if (*it == ref<Node>(this)) {
			(*nodeIt)->m_connections.erase(it);
			break;
		}
	}

	// Remove connection from this node to specified node
	return m_connections.erase(nodeIt);
}

void Node::obstructionEvent(bool obstructed)
{
	m_obstructed = obstructed;

	if (obstructed) {
		// Remove cardinal nodes diagonal connections around this node
		removeConnection(m_grid[m_row - 1][m_col], m_grid[m_row][m_col - 1]);
		removeConnection(m_grid[m_row][m_col - 1], m_grid[m_row + 1][m_col]);
		removeConnection(m_grid[m_row + 1][m_col], m_grid[m_row][m_col + 1]);
		removeConnection(m_grid[m_row][m_col + 1], m_grid[m_row - 1][m_col]);

		// Remove connections with this node
		auto it = m_connections.begin();
		while (it != m_connections.end()) {
			it = removeConnection(it);
		}
	} else {
		// Reconnect the now unobstructed node
		for (int relR = -1; relR <= 1; ++relR) {
			for (int relC = -1; relC <= 1; ++relC) {
				if (relR == 0 && relC == 0)
					continue;

				size_t row = m_row + relR;
				size_t col = m_col + relC;
				ref<Node> node = m_grid[row][col];

				if (m_grid.areConnectable(this, node))
					connect(node);
			}
		}

		// Recconnect cardinal nodes diagonal connections around this node
		ref<Node> node1 = m_grid[m_row - 1][m_col];
		ref<Node> node2 = m_grid[m_row][m_col - 1];
		ref<Node> node3 = m_grid[m_row + 1][m_col];
		ref<Node> node4 = m_grid[m_row][m_col + 1];
		if (m_grid.areConnectable(node1, node2))
			connect(node1, node2);
		if (m_grid.areConnectable(node2, node3))
			connect(node2, node3);
		if (m_grid.areConnectable(node3, node4))
			connect(node3, node4);
		if (m_grid.areConnectable(node4, node1))
			connect(node4, node1);
	}
}
