#ifndef GRID_H
#define GRID_H

#include <vector>
#include <memory>
#include <random>

#include "Cell.h"

extern std::mt19937 myRandomSeed;

class Grid {
private:

	// symbols for visuals
	const char MINESYMBOL = '*';
	const char MARKED_SYMBOL = '#'; // 'M' could also work
	const char NO_MINES_AROUND_SYMBOL = char(254);
	const char NOT_VISIBLE_SYMBOL = ' ';
	const char ERROR_SYMBOL = 'E';

	int gridSize;
	int numOfMines;
	int numOfMarkedMines = 0;
	int numOfWronglyMarkedMines = 0;
	int numOfVisibleCells = 0;
	std::vector< std::vector< std::unique_ptr<Cell> > > cells;



	std::vector< std::vector< std::unique_ptr<Cell> > > initCells(const int gridSize);

	void chooseRandomMineCells(std::vector<int>& mineSpots, const int initChosenX, const int initChosenY) const;

	void createMine(const int X, const int Y);

	void incrNumsAroundMine(const int X, const int Y);

	void printCharRow() const;

	void printLineRow() const;

	void printRow(const int RowNumber) const;

	char cellPrintSymbol(const int X, const int Y) const;

	bool checkAroundCoordinate(const int X, const int Y);

public:

	Grid(int gridSize, int numOfMines);

	void createMinesAndNums(const int initChosenX, const int initChosenY);

	void printGrid() const;
	
	void printSolution() const;

	// to check user given coordinates, and make it visible
	bool checkInputCoordinates(const int X, const int Y);

	// to mark (or unmark) given coordinates, and keeping track of marked and wrongly marked mines
	void markInputCoordinates(const int X, const int Y);

	bool allMinesMarked() const;

	bool allNonMinesVisible() const;

};

#endif