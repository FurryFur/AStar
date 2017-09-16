#include <vector>
#include <chrono>
#include <thread>
#include <math.h>

#include "PathFinder.h"
#include "Node.h"


PathFinder::PathFinder()
{
}


PathFinder::~PathFinder()
{
}

void PathFinder::setStartNode(nanogui::ref<Node> node)
{
	if (!node)
		return;

	clearResults();

	if (m_startNode)
		m_startNode->setFillColor(nvgRGBA(0, 0, 0, 0));

	node->setFillColor(nvgRGBA(0, 100, 200, 180));
	m_startNode = std::move(node);
}

void PathFinder::setEndNode(nanogui::ref<Node> node)
{
	if (!node)
		return;

	clearResults();

	if (m_endNode)
		m_endNode->setFillColor(nvgRGBA(0, 0, 0, 0));

	node->setFillColor(nvgRGBA(100, 0, 200, 180));
	m_endNode = std::move(node);
}

bool PathFinder::isStart(const Node * node) const
{
	if (!node)
		return false;

	return node == m_startNode.get();
}

bool PathFinder::isEnd(const Node * node) const
{
	if (!node)
		return false;

	return node == m_endNode.get();
}

void PathFinder::calculatePath()
{
	if (!m_startNode || !m_endNode)
		return;

	clearResults();

	using PQContainerT = std::vector<NodePriorityPair>;
	std::priority_queue<NodePriorityPair, PQContainerT, PriorityComparator> frontier;

	// Add starting values to pathing graph
	frontier.emplace(m_startNode, 0);
	m_cameFrom[m_startNode] = nullptr;
	m_costSoFar[m_startNode] = 0;

	Node* curNode = nullptr;
	while (!frontier.empty()) {
		if (curNode)
			curNode->setStrokeColor(nvgRGBA(255, 0, 0, 255));

		NodePriorityPair np = frontier.top();
		curNode = getNode(np);
		frontier.pop();

		curNode->setStrokeColor(nvgRGBA(255, 255, 0, 255));
		if (curNode != m_startNode && curNode != m_endNode)
			curNode->setFillColor(nvgRGBA(255 / getCost(np) / 2, 0, 100, 50));
		
		if (curNode == m_endNode)
			break;

		// Explore neighbor nodes
		auto nextIt = curNode->getConnectionListBegin();
		auto end = curNode->getConnectionListEnd();
		for ( ; nextIt != end; ++nextIt) {
			Node* nextNode = *nextIt;
			float pathCost = getLinkCost(curNode, nextNode) + m_costSoFar.at(curNode);

			// Check if we should add / update node in path graph
			auto existingCostIt = m_costSoFar.find(nextNode);
			bool notExplored = existingCostIt == m_costSoFar.end();
			if (notExplored || pathCost < m_costSoFar.at(nextNode)) {
				// Update path graph
				m_cameFrom[nextNode] = curNode;
				m_costSoFar[nextNode] = pathCost;

				// Calculate priority and add to frontier
				float priority = pathCost + heuristic(*nextNode, *m_endNode);
				frontier.emplace(nextNode, priority);

				if (nextNode != m_startNode && nextNode != m_endNode)
					nextNode->setFillColor(nvgRGBA(255 / (priority / 2), 0, 100, 200));

				std::this_thread::sleep_for(std::chrono::milliseconds(100));

				if (nextNode != m_startNode && nextNode != m_endNode)
					nextNode->setFillColor(nvgRGBA(255 / (priority / 2), 0, 100, 150));
			}
		}
	}

	displayResults();
}

void PathFinder::clearResults()
{
	for (auto nodePair : m_cameFrom) {
		Node* node = nodePair.first;
		if (node != m_startNode && node != m_endNode) {
			node->setFillColor(nvgRGBA(0, 0, 0, 0));
			node->setStrokeColor(nvgRGBA(255, 0, 0, 255));
		}
	}

	m_cameFrom.clear();
	m_costSoFar.clear();
}

void PathFinder::displayResults()
{
	/*for (auto nodePair : m_cameFrom) {
		Node* node = nodePair.first;
		if (node != m_startNode && node != m_endNode)
			node->setFillColor(nvgRGBA(0, 0, 100, 50));
	}*/

	Node* curNode = m_endNode;
	while (curNode) {
		curNode->setStrokeColor(nvgRGBA(255, 255, 0, 255));
		curNode = m_cameFrom.at(curNode);
	}
}

float PathFinder::getLinkCost(const Node* nodeSrc, const Node* nodeDst)
{
	if (!nodeSrc || !nodeDst)
		return 0;

	return euclideanDist(*nodeSrc, *nodeDst);
}

float PathFinder::heuristic(const Node& nodeSrc, const Node& nodeDst)
{
	return euclideanDist(nodeSrc, nodeDst);
}

float PathFinder::manhattanDist(const Node& nodeSrc, const Node& nodeDst)
{
	return std::abs(static_cast<float>(nodeDst.getRow()) - static_cast<float>(nodeSrc.getRow()))
	     + std::abs(static_cast<float>(nodeDst.getCol()) - static_cast<float>(nodeSrc.getCol()));
}

float PathFinder::euclideanDist(const Node& nodeSrc, const Node& nodeDst)
{
	return sqrt(std::pow(static_cast<float>(nodeDst.getRow()) - static_cast<float>(nodeSrc.getRow()), 2)
	          + std::pow(static_cast<float>(nodeDst.getCol()) - static_cast<float>(nodeSrc.getCol()), 2));
}

Node* PathFinder::getNode(const NodePriorityPair& nodeCostPair)
{
	return nodeCostPair.first;
}

float PathFinder::getCost(const NodePriorityPair& nodeCostPair)
{
	return nodeCostPair.second;
}

bool PathFinder::PriorityComparator::operator()(const NodePriorityPair& lhs, const NodePriorityPair& rhs) const
{
	return getCost(lhs) > getCost(rhs);
}
