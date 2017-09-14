#include <algorithm>

#include "Node.h"
#include "Grid.h"

using namespace nanogui;

Node::Node(Window * window, size_t row, size_t col, Grid& grid) :
	Widget(window),
	m_grid{ grid },
	m_obstructed{ false } 
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
	}
	else {
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
		obstructionEvent(true);
		return true;
	}
	else if (button == GLFW_MOUSE_BUTTON_2 && down) {
		obstructionEvent(false);
		return true;
	}

	return Widget::mouseButtonEvent(p, button, down, modifiers);
}

bool Node::mouseEnterEvent(const Vector2i & p, bool enter)
{
	Widget::mouseEnterEvent(p, enter);

	if (enter && glfwGetMouseButton(this->screen()->glfwWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		obstructionEvent(true);
		return true;
	}
	else if (enter && glfwGetMouseButton(this->screen()->glfwWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		obstructionEvent(false);
		return true;
	}

	return false;
}

void Node::addConnection(ref<Node> node)
{
	if (std::find(m_connections.begin(), m_connections.end(), node) == m_connections.end()) {
		node->m_connections.push_back(this);
		m_connections.push_back(std::move(node));
	}
}

std::list<ref<Node>>::iterator Node::removeConnection(ref<Node> node)
{
	// Remove connection to node
	for (auto it = node->m_connections.begin(); it != node->m_connections.end(); ++it) {
		if (*it == ref<Node>(this)) {
			node->m_connections.erase(it);
			break;
		}
	}

	// Remove connection from node
	auto it = m_connections.begin();
	while (it != m_connections.end()) {
		if (*it == node) {
			it = m_connections.erase(it);
			break;
		} else {
			++it;
		}
	}

	return it;
}

void Node::obstructionEvent(bool obstructed)
{
	m_obstructed = obstructed;

	


	
	if (obstructed) {
		// TODO: Remove neighboring nodes diagonal connections around this node
		

		// Remove connections with this node
		auto it = m_connections.begin();
		while (it != m_connections.end()) {
			it = removeConnection(*it);
		}
	}
}
