#pragma once

class Node;
class Grid;

namespace nanogui {
	template<class T>
	class ref;
}

class NavPainter {
public:
	enum BrushType {
		Start,
		End,
		Obstacle
	};

	NavPainter(Grid& grid);
	~NavPainter();

	bool paintEvent(int button, nanogui::ref<Node> node);
	void setCurrentBrush(BrushType brush);

private:
	void paintObstacle(Node& node);
	void clearObstacle(Node& node);

	BrushType m_currentBrush;
	Grid& m_grid;
};

