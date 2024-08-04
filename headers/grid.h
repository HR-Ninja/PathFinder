#pragma once
#include "raylib.h"
#include "vector"

enum CellState {
	EMPTY,
	START,
	END,
	BLOCK,
	PATH,
	SEARCHING,
	VISITED
};

class Grid
{
public:
	Grid(const int& width, const int& height, const int& cellSize);
	~Grid();
	void Reset();
	void Draw();
	void UpdateCellState(const int& x, const int& y, const CellState& state);
	CellState GetCellState(const int& x, const int& y);
	int GetCellX(const int& x);
	int GetCellY(const int& y);
	int GetRows();
	int GetColumns();
	int GetCellSize();

private:
	int m_cellSize;
	int m_rows;
	int m_columns;


	std::vector<std::vector<CellState>> m_cells;
};