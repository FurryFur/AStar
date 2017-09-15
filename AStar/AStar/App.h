#pragma once

#include <nanogui\nanogui.h>

#include "Grid.h"

class AStarApp : public nanogui::Screen {
public:
	// Setup UI and Shader
	AStarApp();

	// Draw opengl stuff (screen will handle clear and buffer swap)
	virtual void drawContents() override;

	// Draw nanonvg (vector graphics) stuff
	virtual void draw(NVGcontext* ctx) override;

private:

	enum BrushType {
		Start,
		End,
		Obstacle
	};

	BrushType m_currentBrush;
	nanogui:: GLShader m_shader;
	float m_modulation;
	Grid m_grid;
};