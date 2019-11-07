//Includes
#pragma once;
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <math.h>


#include "save_png.h"
#include "Cell.h"
#include "Timer.h"

#include "DepthFirstSearch.h"

using namespace std;

struct Cell;
class Timer;

class Maze_DepthFirstSearch;

void ExportMaze(const char* filename, const Maze_DepthFirstSearch& mazeRef, const unsigned squareSize);
bool TestBorder(unsigned x, unsigned y, unsigned size, const Cell &cellRef);
bool CheckSteps(const Cell &i, const Cell &j);





//-------------------------------------------
// Functions
//-------------------------------------------
int main()
{
	unsigned width = 90, height = 45;
	const unsigned SQUARE_SIZE = 6;

	cout << "Creating cells... width: " << width << " height: " << height << " cell size: " << SQUARE_SIZE << endl;
	Timer t = Timer();
	t.Start();
	Maze_DepthFirstSearch maze = Maze_DepthFirstSearch(width, height);
	cout << "Creation time: " << to_string(t.GetTime()) << endl;

	cout << endl << "Generating maze..." << endl;
	t.Start();
	maze.Generate();
	cout << "Generation time: " << to_string(t.GetTime()) << endl;

	cout << endl << "Calculating steps..." << endl;
	t.Start();
	maze.CalculateSteps();
	cout << "Step caclculation time: " << to_string(t.GetTime()) << endl;

	cout << endl << "Exporting image..." << endl;
	t.Start();
	ExportMaze("maze.png", maze, SQUARE_SIZE);
	cout << "Image saved." << endl;
	cout << "Image export time: " << to_string(t.GetTime()) << endl;

	cout << endl <<"Press ENTER to close the program" << endl;
	cin.get();
	return 0;
}

