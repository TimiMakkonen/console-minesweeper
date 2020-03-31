#ifndef GRID_H
#define GRID_H

#include <vector>
#include <memory>
#include <random>

#include "Cell.h"

extern std::mt19937 myRandomSeed;

namespace Minesweeper {

	class Grid {
	private:

		int gridHeight;
		int gridWidth;
		int numOfMines;
		int numOfMarkedMines = 0;
		int numOfWronglyMarkedMines = 0;
		int numOfVisibleCells = 0;
		bool _checkedMine = false;

		std::vector< std::vector< std::unique_ptr<Cell> > > cells;


		std::vector< std::vector< std::unique_ptr<Cell> > > initCells();

		void chooseRandomMineCells(std::vector<int>& mineSpots, const int initChosenX, const int initChosenY) const;

		void createMine(const int X, const int Y);

		void incrNumsAroundMine(const int X, const int Y);

		void checkAroundCoordinate(const int X, const int Y);

		bool allMinesMarked() const;

		bool allNonMinesVisible() const;

		bool checkedMine() const;

	public:

		Grid(int gridSize, int numOfMines);

		Grid(int gridHeight, int gridWidth, int numOfMines);

		void createMinesAndNums(const int initChosenX, const int initChosenY);

		// to check user given coordinates, and make it visible
		void checkInputCoordinates(const int X, const int Y);

		// to mark (or unmark) given coordinates, and keeping track of marked and wrongly marked mines
		void markInputCoordinates(const int X, const int Y);

		bool playerHasWon() const;

		bool playerHasLost() const;

		bool isCellVisible(const int X, const int Y) const;

		bool doesCellHaveMine(const int X, const int Y) const;

		bool isCellMarked(const int X, const int Y) const;

		int numOfMinesAroundCell(const int X, const int Y) const;

	};

}

#endif