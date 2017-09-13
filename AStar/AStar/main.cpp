#include <iostream>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/glutil.h>
#include <nanogui/label.h>
#include <nanogui/theme.h>
#include <nanogui/formhelper.h>
#include <nanogui/slider.h>

using namespace nanogui;

class Cell : public Widget {
public:
	Cell(Window* window) :
		Widget(window),
		m_toggle{ false } { }

	virtual void draw(NVGcontext* ctx) override
	{
		nvgBeginPath(ctx);
		nvgRect(ctx, mPos.x() - 0.5f, mPos.y() - 0.5f, mSize.x() + 1, mSize.y() + 1);
		if (m_toggle) {
			nvgFillColor(ctx, nvgRGBA(255, 0, 0, 255));
			nvgFill(ctx);
		} else {
			nvgStrokeWidth(ctx, 5.0f);
			nvgStrokeColor(ctx, nvgRGBA(255, 0, 0, 255));
			nvgStroke(ctx);
		}

		Widget::draw(ctx);
	}

	virtual bool mouseButtonEvent(const Vector2i& p, int button, bool down, int modifiers) override
	{
		if (button == GLFW_MOUSE_BUTTON_1 && down) {
			m_toggle = true;
			return true;
		} else if (button == GLFW_MOUSE_BUTTON_2 && down) {
			m_toggle = false;
			return true;
		}

		return Widget::mouseButtonEvent(p, button, down, modifiers);
	}

	virtual bool mouseEnterEvent(const Vector2i& p, bool enter) override 
	{
		Widget::mouseEnterEvent(p, enter);

		if (enter && glfwGetMouseButton(this->screen()->glfwWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
			m_toggle = true;
			return true;
		} else if (enter && glfwGetMouseButton(this->screen()->glfwWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
			m_toggle = false;
			return true;
		}
	}

private:
	bool m_toggle;
};

class AStarApp : public Screen {
public:
	AStarApp() : 
		m_modulation{ 5 },
		Screen(Vector2i(1024, 850), "AStar")
	{
		/**
		* Add a window.
		* To the window add a label and a slider widget.
		*/

		Window* window = new Window(this, "");
		window->setPosition({ 15, 15 });
		window->setLayout(new GroupLayout(5, 5, 0, 0));

		Label* l = new Label(window, "MODULATION", "sans-bold");
		l->setFontSize(10);
		auto slider = new Slider(window);
		slider->setValue(0.5f);
		slider->setCallback([this](float value) { m_modulation = value * 10.0f; });

		Window* window2 = new Window(this, "");
		window2->setPosition({ 100, 15 });
		int gridSize = 16;
		window2->setLayout(new GridLayout(Orientation::Horizontal, gridSize));

		for (int i = 0; i < gridSize * gridSize; ++i) {
			auto cell = new Cell(window2);
			cell->setFixedSize({ 50, 50 });
		}

		// Do the layout calculations based on what was added to the GUI
		performLayout();

		// Load GLSL shader code
		m_shader.initFromFiles("raymarching_shader", "Shaders/vert.glsl", "Shaders/frag.glsl");

		/**
		* Fill the screen with a rectangle (2 triangles)
		*/
		MatrixXu indices(3, 2);
		indices.col(0) << 0, 1, 2;
		indices.col(1) << 2, 1, 3;

		MatrixXf positions(3, 4);
		positions.col(0) << -1, -1, 0;
		positions.col(1) << 1, -1, 0;
		positions.col(2) << -1, 1, 0;
		positions.col(3) << 1, 1, 0;

		// bind the shader and upload vertex positions and indices
		m_shader.bind();
		m_shader.uploadIndices(indices);
		m_shader.uploadAttrib("a_position", positions);

		// Set initial value for modulation uniform
		m_shader.setUniform("modulation", m_modulation);

		// Set resolution and screenRatio uniforms
		int fboWidth, fboHeight;
		glfwGetFramebufferSize(mGLFWWindow, &fboWidth, &fboHeight);
		m_shader.setUniform("resolution", Vector2f{ fboWidth, fboHeight });
		float mx = std::max<float>(fboWidth, fboHeight);
		auto xDim = fboWidth / mx;
		auto yDim = fboHeight / mx;
		m_shader.setUniform("screenRatio", Vector2f{ xDim, yDim });
	}

	// Draw opengl stuff (screen will handle clear and buffer swap)
	virtual void drawContents() override 
	{
		m_shader.bind();
		m_shader.setUniform("modulation", m_modulation);
		m_shader.drawIndexed(GL_TRIANGLES, 0, 2);
	}

	// Draw nanonvg (vector graphics) stuff
	virtual void draw(NVGcontext* ctx)
	{
		//nvgBeginPath(ctx);
		//nvgRect(ctx, 100, 100, 120, 30);
		//nvgCircle(ctx, 120, 120, 5);
		//nvgPathWinding(ctx, NVG_HOLE);	// Mark circle as a hole.
		//nvgFillColor(ctx, nvgRGBA(255, 192, 0, 255));
		//nvgFill(ctx);

		Screen::draw(ctx);
	}

private:
	GLShader m_shader;
	float m_modulation;
};

int main() {
	nanogui::init();

	AStarApp app;
	app.setVisible(true);

	nanogui::mainloop();

	nanogui::shutdown();
	exit(EXIT_SUCCESS);
}