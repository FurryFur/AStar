#include "PathFinder.h"



PathFinder::PathFinder()
{
}


PathFinder::~PathFinder()
{
}

void PathFinder::setStartNode(nanogui::ref<Node> node)
{
	m_startNode = std::move(node);
}

void PathFinder::setEndNode(nanogui::ref<Node> node)
{
	m_endNode = std::move(node);
}
