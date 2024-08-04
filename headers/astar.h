#pragma once
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include "grid.h"

class AStar
{
public:
    struct Node {
        int x, y;
        float gCost, hCost, fCost;
        int parentX, parentY;
        bool walkAble;

        Node(int x = -1, int y = -1, bool walkAble = true)
            : x(x), y(y), gCost(FLT_MAX), hCost(FLT_MAX), fCost(FLT_MAX), parentX(-1), parentY(-1), walkAble(walkAble) {}

        bool operator==(const Node& other) const {
            return x == other.x && y == other.y;
        }
    };

    float Heuristic(int x1, int y1, int x2, int y2);
    std::vector<Node> GetNeighbors(Node& node, Grid& grid);
    std::vector<Node> ReconstructPath(Node& endNode, std::vector<std::vector<Node>>& allNodes);
    void Initialize(Grid& grid, int startX, int startY, int endX, int endY);
    std::vector<Node> ProcessStep(Grid& grid, int endX, int endY);
    void Reset();

    std::vector<Node> openNodes;
    std::vector<Node> closedNodes;
    std::vector<std::vector<Node>> allNodes;
};