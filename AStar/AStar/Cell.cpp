#include "Cell.h"

Cell::Cell(Window * window) :
	Widget(window),
	m_toggle{ false } { }

void Cell::draw(NVGcontext * ctx)
{
	nvgBeginPath(ctx);
	nvgRect(ctx, static_cast<float>(mPos.x()), static_cast<float>(mPos.y()), static_cast<float>(mSize.x()), static_cast<float>(mSize.y()));
	if (m_toggle) {
		nvgFillColor(ctx, nvgRGBA(255, 0, 0, 255));
		nvgFill(ctx);
	}
	else {
		nvgStrokeWidth(ctx, 5.0f);
		nvgStrokeColor(ctx, nvgRGBA(255, 0, 0, 255));
		nvgStroke(ctx);
	}

	Widget::draw(ctx);
}

bool Cell::mouseButtonEvent(const Vector2i & p, int button, bool down, int modifiers)
{
	if (button == GLFW_MOUSE_BUTTON_1 && down) {
		m_toggle = true;
		return true;
	}
	else if (button == GLFW_MOUSE_BUTTON_2 && down) {
		m_toggle = false;
		return true;
	}

	return Widget::mouseButtonEvent(p, button, down, modifiers);
}

bool Cell::mouseEnterEvent(const Vector2i & p, bool enter)
{
	Widget::mouseEnterEvent(p, enter);

	if (enter && glfwGetMouseButton(this->screen()->glfwWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		m_toggle = true;
		return true;
	}
	else if (enter && glfwGetMouseButton(this->screen()->glfwWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		m_toggle = false;
		return true;
	}

	return false;
}
