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

class AStarApp : public nanogui::Screen {
public:
	AStarApp() : 
		m_modulation{ 5 },
		nanogui::Screen(nanogui::Vector2i(1024, 768), "NanoGUI Test")
	{
		using namespace nanogui;

		/**
		* Add a window.
		* To the window add a label and a slider widget.
		*/

		Window* window = new Window(this, "");
		window->setPosition({ 15, 15 });
		window->setLayout(new GroupLayout(5, 5, 0, 0));

		Label *l = new Label(window, "MODULATION", "sans-bold");
		l->setFontSize(10);
		auto slider = new Slider(window);
		slider->setValue(0.5f);
		slider->setCallback([this](float value) { m_modulation = value * 10.0f; });

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
		nvgBeginPath(ctx);
		nvgRect(ctx, 100, 100, 120, 30);
		nvgCircle(ctx, 120, 120, 5);
		nvgPathWinding(ctx, NVG_HOLE);	// Mark circle as a hole.
		nvgFillColor(ctx, nvgRGBA(255, 192, 0, 255));
		nvgFill(ctx);

		Screen::draw(ctx);
	}

private:
	nanogui::GLShader m_shader;
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