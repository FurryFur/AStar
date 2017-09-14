#pragma once

#include <nanogui\nanogui.h>

#include "Cell.h"

using namespace nanogui;

class AStarApp : public Screen {
public:
	// Setup UI and Shader
	AStarApp();

	// Draw opengl stuff (screen will handle clear and buffer swap)
	virtual void drawContents() override;

	// Draw nanonvg (vector graphics) stuff
	virtual void draw(NVGcontext* ctx) override;

private:
	GLShader m_shader;
	float m_modulation;
};