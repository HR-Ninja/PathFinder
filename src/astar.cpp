#include "astar.h"


float AStar::Heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

std::vector<AStar::Node> AStar::GetNeighbors(Node& node, Grid& grid) {
    std::vector<Node> neighbors;
    std::vector<std::pair<int, int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    for (const auto& dir : directions) {
        int newX = node.x + dir.first;
        int newY = node.y + dir.second;

        if (newX >= 0 && newX < grid.GetColumns() && newY >= 0 && newY < grid.GetRows() &&
            grid.GetCellState(newX * grid.GetCellSize(), newY * grid.GetCellSize()) != BLOCK) {
            neighbors.push_back(Node(newX, newY, true));
        }
    }

    return neighbors;
}

std::vector<AStar::Node> AStar::ReconstructPath(Node& endNode, std::vector<std::vector<Node>>& allNodes) {
    std::vector<Node> path;
    Node* currentNode = &endNode;

    while (currentNode->parentX != -1 && currentNode->parentY != -1) {
        path.push_back(*currentNode);
        currentNode = &allNodes[currentNode->parentY][currentNode->parentX];
    }
    path.push_back(*currentNode);

    std::reverse(path.begin(), path.end());
    return path;
}

void AStar::Initialize(Grid& grid, int startX, int startY, int endX, int endY) {
    allNodes = std::vector<std::vector<Node>>(grid.GetRows(), std::vector<Node>(grid.GetColumns(), Node(-1, -1, false)));

    for (int y = 0; y < grid.GetRows(); y++) {
        for (int x = 0; x < grid.GetColumns(); x++) {
            allNodes[y][x] = Node(x, y, grid.GetCellState(x * grid.GetCellSize(), y * grid.GetCellSize()) != BLOCK);
        }
    }

    Node& startNode = allNodes[startY][startX];
    startNode.gCost = 0;
    startNode.hCost = Heuristic(startX, startY, endX, endY);
    startNode.fCost = startNode.gCost + startNode.hCost;

    openNodes.clear();
    closedNodes.clear();
    openNodes.push_back(startNode);
}

std::vector<AStar::Node> AStar::ProcessStep(Grid& grid, int endX, int endY) {
    if (openNodes.empty()) {
        return std::vector<Node>(); // No path found
    }

    auto cmp = [](Node& left, Node& right) { return left.fCost > right.fCost; };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> openSet(cmp, openNodes);

    Node currentNode = openSet.top();
    openSet.pop();
    openNodes.erase(std::remove(openNodes.begin(), openNodes.end(), currentNode), openNodes.end());

    if (currentNode.x == endX && currentNode.y == endY) {
        return ReconstructPath(currentNode, allNodes);
    }

    closedNodes.push_back(currentNode);

    for (Node& neighbor : GetNeighbors(currentNode, grid)) {
        Node& actualNeighbor = allNodes[neighbor.y][neighbor.x];

        if (std::find(closedNodes.begin(), closedNodes.end(), actualNeighbor) != closedNodes.end()) {
            continue;
        }

        float tentativeGCost = currentNode.gCost + Heuristic(currentNode.x, currentNode.y, actualNeighbor.x, actualNeighbor.y);

        bool inOpenSet = std::find(openNodes.begin(), openNodes.end(), actualNeighbor) != openNodes.end();

        if (tentativeGCost < actualNeighbor.gCost || !inOpenSet) {
            actualNeighbor.gCost = tentativeGCost;
            actualNeighbor.hCost = Heuristic(actualNeighbor.x, actualNeighbor.y, endX, endY);
            actualNeighbor.fCost = actualNeighbor.gCost + actualNeighbor.hCost;
            actualNeighbor.parentX = currentNode.x;
            actualNeighbor.parentY = currentNode.y;

            if (!inOpenSet) {
                openSet.push(actualNeighbor);
                openNodes.push_back(actualNeighbor);
            }
        }
    }

    return std::vector<Node>();
}

void AStar::Reset() {
    openNodes.clear();
    closedNodes.clear();
    allNodes.clear();
}