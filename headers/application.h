#pragma once
#include "raylib.h"
#include "grid.h"
#include "astar.h"

class Application
{
public:
	Application(const int& screenWidth, const int& screenHeight, const char* title);
	void Run();

private:

	AStar aStar;

	int startX;
	int startY;
	int endX;
	int endY;

	bool startPlaced = false;
	bool endPlaced = false;
	bool isRunning = false;

	std::vector<AStar::Node> path;

	void HandleInputs(Grid& grid);
	void Update(Grid& grid);
	void UI();
	void Close();
};