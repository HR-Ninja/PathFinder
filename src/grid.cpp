#include "grid.h"

Grid::Grid(const int& width, const int& height, const int& cellSize)
{
	m_cellSize = cellSize;
	m_rows = height / cellSize;
	m_columns = width / cellSize;
	m_cells.resize(m_rows, std::vector<CellState>(m_columns, EMPTY));
}

Grid::~Grid()
{

}

void Grid::Reset()
{
	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_columns; j++)
		{
			m_cells[i][j] = EMPTY;
		}
	}
}

void Grid::Draw()
{
	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_columns; j++)
		{
			Color color = LIGHTGRAY;

			if (m_cells[i][j] == START)
			{
				color = GREEN;
			}
			else if (m_cells[i][j] == END)
			{
				color = RED;
			}
			else if (m_cells[i][j] == BLOCK)
			{
				color = BLACK;
			}
			else if (m_cells[i][j] == VISITED)
			{
				color = DARKGRAY;
			}
			else if (m_cells[i][j] == SEARCHING)
			{
				color = BLUE;
			}
			else if (m_cells[i][j] == PATH)
			{
				color = YELLOW;
			}

			DrawRectangle(j * m_cellSize, i * m_cellSize, m_cellSize - 1, m_cellSize - 1, color);
		}
	}
}

void Grid::UpdateCellState(const int& x, const int& y, const CellState& state)
{
	if (y >= 0 && y < m_rows && x >= 0 && x < m_columns) {
		m_cells[y][x] = state;
	}
}


CellState Grid::GetCellState(const int& x, const int& y)
{
	int column = x / m_cellSize;
	int row = y / m_cellSize;

	if (row >= 0 && row < m_rows && column >= 0 && column < m_columns) {
		return m_cells[row][column];
	}

	return EMPTY;
}

int Grid::GetCellX(const int& x)
{
	return x / m_cellSize;
}

int Grid::GetCellY(const int& y)
{
	return y / m_cellSize;
}

int Grid::GetRows()
{
	return m_rows;
}

int Grid::GetColumns()
{
	return m_columns;
}

int Grid::GetCellSize()
{
	return m_cellSize;
}