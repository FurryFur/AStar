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
	void draw(NVGcontext* ctx);
	void clear();
	
private:
	using NodePriorityPair = std::pair<Node*, float>;
	using PQContainerT = std::vector<NodePriorityPair>;

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

	void fillNode(NVGcontext* ctx, const Node& node, const NVGcolor& color);
	void strokeNode(NVGcontext* ctx, const Node& node, const NVGcolor& color);
	void drawGraphSegment(NVGcontext* ctx, const Node& nodeFrom, const Node& nodeTo, const NVGcolor& color);

	nanogui::ref<Node> m_startNode;
	nanogui::ref<Node> m_endNode;
	Node* m_curNode;
	Node* m_nextNode;
	std::priority_queue<NodePriorityPair, PQContainerT, PriorityComparator> m_frontier;
	std::unordered_map<Node*, Node*> m_cameFrom;
	std::unordered_map<Node*, float> m_costSoFar;
};