#pragma once

#include <utility>
#include <queue>
#include <unordered_map>

#include <nanogui\object.h>

class Node;

class PathFinder {
public:
	PathFinder();
	~PathFinder();

	void setStartNode(nanogui::ref<Node> node);
	void setEndNode(nanogui::ref<Node> node);
	
private:
	nanogui::ref<Node> m_startNode;
	nanogui::ref<Node> m_endNode;
	std::priority_queue <std::pair<size_t, Node*>> frontier;
	std::unordered_map<Node*, Node*> cameFrom;
	std::unordered_map<Node*, size_t> costSoFar;
};

