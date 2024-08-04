#include "application.h"
#include <iostream>
#include <chrono>
#include <thread>

Application::Application(const int& screenWidth, const int& screenHeight, const char* title)
{
    InitWindow(screenWidth, screenHeight, title);
    SetTargetFPS(60);
    startX = 0;
    startY = 0;
    endX = 0;
    endY = 0;
}

void Application::Run()
{
    Grid grid(800, 700, 20);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        HandleInputs(grid);
        Update(grid);
        grid.Draw();

        UI();
        EndDrawing();
    }

    Close();
}

void Application::Update(Grid& grid)
{
    if (isRunning) {
        if (!aStar.openNodes.empty()) {
            // Process the next step in the A* algorithm
            path = aStar.ProcessStep(grid, endX, endY);

            // Draw the open and closed nodes
            for (const auto& node : aStar.openNodes) {
                grid.UpdateCellState(node.x, node.y, SEARCHING);
            }

            for (const auto& node : aStar.closedNodes) {
                grid.UpdateCellState(node.x, node.y, VISITED);
            }

            // If path is found, stop running and draw the path in blue
            if (!path.empty() && path.back().x == endX && path.back().y == endY) {
                isRunning = false;
            }

            // Maintain start and end point colors
            grid.UpdateCellState(startX, startY, START);
            grid.UpdateCellState(endX, endY, END);

            // Delay to slow down the visualization
            std::this_thread::sleep_for(std::chrono::milliseconds(20));  // Adjust the delay as needed
        }
    }

    if (!isRunning && !path.empty()) {
        // Draw the final path in blue
        for (const auto& node : path) {
            grid.UpdateCellState(node.x, node.y, PATH);
        }

        // Maintain start and end point colors
        grid.UpdateCellState(startX, startY, START);
        grid.UpdateCellState(endX, endY, END);

    }
}

void Application::HandleInputs(Grid& grid)
{
    int cellsize = grid.GetCellSize();

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        Vector2 mousePosition = GetMousePosition();
        if (grid.GetCellState(mousePosition.x, mousePosition.y) == EMPTY && !startPlaced)
        {
            grid.UpdateCellState(mousePosition.x / cellsize, mousePosition.y / cellsize, START);
            startX = grid.GetCellX(mousePosition.x);
            startY = grid.GetCellY(mousePosition.y);
            startPlaced = true;
        }
        else if (grid.GetCellState(mousePosition.x, mousePosition.y) == EMPTY && startPlaced && !endPlaced)
        {
            grid.UpdateCellState(mousePosition.x / cellsize, mousePosition.y / cellsize, END);
            endX = grid.GetCellX(mousePosition.x);
            endY = grid.GetCellY(mousePosition.y);
            endPlaced = true;
        }
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePosition = GetMousePosition();
        if (grid.GetCellState(mousePosition.x / cellsize, mousePosition.y / cellsize) == EMPTY)
        {
            grid.UpdateCellState(mousePosition.x / cellsize, mousePosition.y / cellsize, BLOCK);
        }
    }

    if (IsKeyPressed(KEY_R))
    {
        path.clear();
        grid.Reset();
        startPlaced = false;
        endPlaced = false;
        isRunning = false;
        aStar.Reset();
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        if (startPlaced && endPlaced) {
            aStar.Initialize(grid, startX, startY, endX, endY);
            isRunning = true;
        }
    }
}

void Application::UI()
{
    DrawText("A* Path Finding Visualizer", 400, 740, 20, RAYWHITE);

    DrawText("Controls: ", 10, 710, 16, LIGHTGRAY);
    DrawText("Left Mouse Button to place: Obstacles", 10, 730, 14, LIGHTGRAY);
    DrawText("Right Mouse Button to place: Start/End", 10, 745, 14, LIGHTGRAY);
    DrawText("R Button to reset", 10, 760, 14, LIGHTGRAY);
    DrawText("SPACE Button to begin", 10, 775, 14, LIGHTGRAY);
}

void Application::Close()
{
    CloseWindow();
}
