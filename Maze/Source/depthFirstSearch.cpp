#include "DepthFirstSearch.h"

#include <random>
#include <math.h>

#include "Cell.h"
#include "save_png.h"
#include "Timer.h"

bool CheckSteps(const Cell& i, const Cell& j);
bool TestBorder(unsigned x, unsigned y, unsigned size, const Cell& cellRef);
void ExportMaze(const char* filename, const Maze_DepthFirstSearch& maze, const unsigned squareSize);


unsigned Maze_DepthFirstSearch::GetMostSteps() const
{
	return (*std::max_element(m_Data.begin(), m_Data.end(), CheckSteps)).steps;
}

void Maze_DepthFirstSearch::Generate()
{
	random_device rd;
	mt19937 genIndex = std::mt19937(rd());
	mt19937 genOrientation = std::mt19937(rd());

	uniform_int_distribution<unsigned> disIndex = std::uniform_int_distribution<unsigned>(0, m_Width * m_Height - 1);
	uniform_int_distribution<unsigned> disOrientation;

	unsigned index = disIndex(genIndex);
	m_Data[index].isAssigned = true;
	m_Elegible.push_back(index);
	vector<unsigned> availableStartCells;
	array<unsigned, 4> availableNeighbours;
	unsigned size = 1;

	while (size > 0)
	{
		//Get available neighbours
		char neighbourSize = GetAvailableNeighbours(availableNeighbours, index);
		while (neighbourSize > 0)
		{
			//Pick random neighbour
			disOrientation = std::uniform_int_distribution<unsigned>(0, neighbourSize - 1);
			unsigned neighbourIdx = availableNeighbours[disOrientation(genOrientation)];
			//Remove border to neighbour
			if (neighbourIdx == index - 1)//left
			{
				m_Data[index].left = true;
				m_Data[neighbourIdx].right = true;
			}
			else if (neighbourIdx == index + 1)//right
			{
				m_Data[index].right = true;
				m_Data[neighbourIdx].left = true;
			}
			else if (neighbourIdx == index - m_Width)//up
			{
				m_Data[index].up = true;
				m_Data[neighbourIdx].down = true;
			}
			else//down
			{
				m_Data[index].down = true;
				m_Data[neighbourIdx].up = true;
			}
			//Go to neighbour
			index = neighbourIdx;
			m_Data[index].isAssigned = true;
			m_Elegible.push_back(index);
			//Get available neighbours
			neighbourSize = GetAvailableNeighbours(availableNeighbours, index);
		}
		//Get next step
		GetAvailableCells(availableStartCells);
		size = availableStartCells.size();
		disIndex = std::uniform_int_distribution<unsigned>(0, size - 1);
		if (size > 0) index = availableStartCells[disIndex(genIndex)];
	}
}


void Maze_DepthFirstSearch::CalculateSteps()
{
	unsigned index = 0;
	m_Data[index].steps = 0;
	unsigned steps = 1;
	if (m_Data[index].right)
	{
		SetStepsAndIterate(index + 1, steps, 3);
	}
	if (m_Data[index].down)
	{
		SetStepsAndIterate(index + m_Width, steps, 0);
	}
}

void Maze_DepthFirstSearch::Precalculate()
{
	size_t size = m_Data.size();
	for (size_t i = 0; i < size; i++)
	{
		if (i < m_Width) m_Data[i].nbUp = string::npos;
		else m_Data[i].nbUp = i - m_Width;

		if (i % m_Width == m_Width - 1) m_Data[i].nbRight = string::npos;
		else m_Data[i].nbRight = i + 1;

		if (i / m_Width == m_Height - 1) m_Data[i].nbDown = string::npos;
		else m_Data[i].nbDown = i + m_Width;

		if (i % m_Width == 0) m_Data[i].nbLeft = string::npos;
		else m_Data[i].nbLeft = i - 1;
	}
}
void Maze_DepthFirstSearch::GetAvailableCells(std::vector<unsigned>& indices)
{
	indices.clear();
	indices.reserve(m_Height * m_Height);

	for (size_t i = 0; i < m_Elegible.size(); i++)
	{
		unsigned idx = m_Elegible[i];
		if (IsCellAvailable(idx))
		{
			indices.push_back(idx);
		}
	}
}
bool Maze_DepthFirstSearch::IsCellAvailable(unsigned idx)
{
	Cell c = m_Data[idx];
	if (!(c.nbLeft == string::npos))
		if (!(m_Data[c.nbLeft].isAssigned)) return true;
	if (!(c.nbUp == string::npos))
		if (!(m_Data[c.nbUp].isAssigned)) return true;
	if (!(c.nbRight == string::npos))
		if (!(m_Data[c.nbRight].isAssigned)) return true;
	if (!(c.nbDown == string::npos))
		if (!(m_Data[c.nbDown].isAssigned)) return true;
	return false;
}
char Maze_DepthFirstSearch::GetAvailableNeighbours(array<unsigned, 4>& indices, unsigned idx)
{
	char ret = 0;
	Cell c = m_Data[idx];
	if (!(c.nbLeft == string::npos))
	{
		if (!(m_Data[c.nbLeft].isAssigned))
		{
			indices[0] = c.nbLeft;
			++ret;
		}
	}
	if (!(c.nbUp == string::npos))
	{
		if (!(m_Data[c.nbUp].isAssigned))
		{
			indices[ret] = c.nbUp;
			++ret;
		}
	}
	if (!(c.nbRight == string::npos))
	{
		if (!(m_Data[c.nbRight].isAssigned))
		{
			indices[ret] = c.nbRight;
			++ret;
		}
	}
	if (!(c.nbDown == string::npos))
	{
		if (!(m_Data[c.nbDown].isAssigned))
		{
			indices[ret] = c.nbDown;
			++ret;
		}
	}
	return ret;
}
void Maze_DepthFirstSearch::SetStepsAndIterate(unsigned idx, unsigned steps, unsigned from)
{
	m_Data[idx].steps = steps;
	steps++;
	Cell c = m_Data[idx];
	if (c.up && !(from == 0)) SetStepsAndIterate(c.nbUp, steps, 2);
	if (c.right && !(from == 1)) SetStepsAndIterate(c.nbRight, steps, 3);
	if (c.down && !(from == 2)) SetStepsAndIterate(c.nbDown, steps, 0);
	if (c.left && !(from == 3)) SetStepsAndIterate(c.nbLeft, steps, 1);
}

///
//Static
///

void ExportMaze(const char* filename, const Maze_DepthFirstSearch& maze, const unsigned squareSize)
{
	unsigned sizeX = maze.GetWidth() * squareSize;
	unsigned sizeY = maze.GetHeight() * squareSize;
	unsigned maxSteps = maze.GetMostSteps();
	vector<Cell> data = maze.GetData();
	vector<char> pixels = vector<char>(sizeX * sizeY * 3, 0);
	unsigned bmpWidth = sizeX * 3;

	for (size_t i = 0; i < data.size(); i++)
	{
		float reachability = (float)data[i].steps / (float)maxSteps;
		float g = min(1.f, (1.f - reachability) * 2);
		float r = min(1.f, reachability * 2);
		char green = (char)(g * 255.f);
		char red = (char)(r * 255.f);

		unsigned idxXBase = (i % maze.GetWidth()) * squareSize;
		unsigned idxYBase = (i / maze.GetWidth()) * squareSize;

		for (size_t x = 0; x < squareSize; x++)
		{
			unsigned idxX = idxXBase + x;
			for (size_t y = 0; y < squareSize; y++)
			{
				unsigned idxY = idxYBase + y;
				unsigned idx = idxY * bmpWidth + idxX * 3;
				if (TestBorder(x, y, squareSize, data[i]))
				{
					pixels[idx + 1] = 0;
					pixels[idx + 2] = 0;
				}
				else
				{
					pixels[idx + 1] = green;
					pixels[idx + 2] = red;
				}
			}
		}
	}

	save_png(filename, pixels.data(), sizeX, sizeY);
}

bool TestBorder(unsigned x, unsigned y, unsigned size, const Cell& cellRef)
{
	if (x == 0)
	{
		if (!cellRef.left) return true;
		else if (y == 0 || y == size - 1) return true;
	}
	else if (x == size - 1)
	{
		if (!cellRef.right) return true;
		else if (y == 0 || y == size - 1) return true;
	}
	if (y == 0)
	{
		if (!cellRef.up) return true;
	}
	else if (y == size - 1)
	{
		if (!cellRef.down) return true;
	}
	return false;
}
bool CheckSteps(const Cell& i, const Cell& j)
{
	return i.steps < j.steps;
}