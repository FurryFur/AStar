#include <algorithm>

#include "Node.h"
#include "Grid.h"
#include "NavPainter.h"

using namespace nanogui;

Node::Node(Window * window, NavPainter& navPainter, size_t row, size_t col)
	: Widget(window)
	, m_navPainter{ navPainter }
	, m_row{ row }
	, m_col{ col }
	, m_obstructed{ false } 
{
}

void Node::draw(NVGcontext * ctx)
{
	nvgSave(ctx);

	nvgBeginPath(ctx);
	nvgRect(ctx, static_cast<float>(mPos.x()), static_cast<float>(mPos.y()), static_cast<float>(mSize.x()), static_cast<float>(mSize.y()));
	if (m_obstructed) {
		nvgFillColor(ctx, nvgRGBA(255, 0, 0, 255));
		nvgFill(ctx);
	} else if (m_isStart) {
		nvgFillColor(ctx, nvgRGBA(0, 100, 200, 255));
		nvgFill(ctx);
	} else if (m_isEnd) {
		nvgFillColor(ctx, nvgRGBA(100, 0, 200, 255));
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
	if ((button == GLFW_MOUSE_BUTTON_1 || button == GLFW_MOUSE_BUTTON_2) && down) {
		m_navPainter.paintEvent(button, this);

		return true;
	}

	return Widget::mouseButtonEvent(p, button, down, modifiers);
}

bool Node::mouseEnterEvent(const Vector2i & p, bool enter)
{
	if (enter) {
		int button1State = glfwGetMouseButton(this->screen()->glfwWindow(), GLFW_MOUSE_BUTTON_1);
		int button2State = glfwGetMouseButton(this->screen()->glfwWindow(), GLFW_MOUSE_BUTTON_2);

		if (button1State == GLFW_PRESS)
			return m_navPainter.paintEvent(GLFW_MOUSE_BUTTON_1, this);
		if (button2State == GLFW_PRESS)
			return m_navPainter.paintEvent(GLFW_MOUSE_BUTTON_2, this);
	}

	return Widget::mouseEnterEvent(p, enter);
}

bool Node::connect(ref<Node> node)
{
	if (!node)
		return false;

	// Add connection to node if one doesn't already exist
	auto it = std::find(m_connections.begin(), m_connections.end(), node);
	if (it == m_connections.end()) {
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

void Node::setIsStart(bool isStart)
{
	m_isStart = isStart;
}

void Node::setIsEnd(bool isEnd)
{
	m_isEnd = isEnd;
}

bool Node::isStart() const
{
	return m_isStart;
}

bool Node::isEnd() const
{
	return m_isEnd;
}

void Node::setObstructed(bool isObstructed)
{
	m_obstructed = isObstructed;
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

std::list<nanogui::ref<Node>>::iterator Node::getConnectionListBegin()
{
	return m_connections.begin();
}

std::list<nanogui::ref<Node>>::iterator Node::getConnectionListEnd()
{
	return m_connections.end();
}
