#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

#include "Cell.h"

using namespace std;

class Maze_DepthFirstSearch
{
public:
	Maze_DepthFirstSearch(unsigned width, unsigned height)
		:m_Width(width)
		, m_Height(height)
	{
		m_Data = std::vector<Cell>(width * height, Cell());
		Precalculate();
	}
	//Data access
	unsigned GetWidth() const { return m_Width; };
	unsigned GetHeight() const { return m_Height; };
	std::vector<Cell> GetData() const { return m_Data; }
	//Operations
	unsigned GetMostSteps() const;

	void Generate();
	void CalculateSteps();
private:
	unsigned m_Width, m_Height;
	std::vector<Cell> m_Data;
	std::vector<unsigned> m_Elegible;

	void Precalculate();
	void GetAvailableCells(std::vector<unsigned>& indices);
	bool IsCellAvailable(unsigned idx);
	char GetAvailableNeighbours(array<unsigned, 4>& indices, unsigned idx);
	void SetStepsAndIterate(unsigned idx, unsigned steps, unsigned from);
};