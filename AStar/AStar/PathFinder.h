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
	bool isStart(const Node* node) const;
	bool isEnd(const Node* node) const;
	void calculatePath();
	
private:
	using NodePriorityPair = std::pair<Node*, float>;

	class PriorityComparator {
	public:
		bool operator()(const NodePriorityPair& node1, const NodePriorityPair& node2) const;
	};

	static float getLinkCost(const Node*, const Node*);
	static float heuristic(const Node&, const Node&);
	static float manhattanDist(const Node&, const Node&);
	static float euclideanDist(const Node&, const Node&);

	static Node* getNode(const NodePriorityPair&);
	static float getCost(const NodePriorityPair&);

	void clearResults();
	void displayResults();

	nanogui::ref<Node> m_startNode;
	nanogui::ref<Node> m_endNode;
	std::unordered_map<Node*, Node*> m_cameFrom;
	std::unordered_map<Node*, float> m_costSoFar;
};