#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <random>
#include <numeric>

#include "Grid.h"


Grid::Grid(int gridSize, int numOfMines) : gridSize(gridSize), numOfMines(numOfMines), cells(this->initCells(gridSize)) {}



std::vector< std::vector< std::unique_ptr<Cell> > > Grid::initCells(const int gridSize) {

	std::vector< std::vector< std::unique_ptr<Cell> > > initTempCells;
	initTempCells.reserve(gridSize);
	for (int i = 0; i < gridSize; ++i) {
		std::vector< std::unique_ptr<Cell> > tempVecOfCells;
		tempVecOfCells.reserve(gridSize);
		for (int j = 0; j < gridSize; ++j) {
			tempVecOfCells.emplace_back(std::make_unique<Cell>());
		}
		initTempCells.emplace_back(std::move(tempVecOfCells));
	}
	return initTempCells;
}



void Grid::createMinesAndNums(const int initChosenX, const int initChosenY) {

	std::vector<int> mineSpots(this->gridSize * this->gridSize);
	this->chooseRandomMineCells(mineSpots, initChosenX, initChosenY);

	int X, Y;
	for (int i = 0; i < this->numOfMines; ++i) {

		X = mineSpots[i] % this->gridSize;
		Y = mineSpots[i] / this->gridSize;

		this->createMine(X, Y);

		this->incrNumsAroundMine(X, Y);
	}
}



void Grid::chooseRandomMineCells(std::vector<int>& mineSpots, const int initChosenX, const int initChosenY) const {

	// to populate mineSpots vector with values: 0, 1, 2, ..., GridSize*GridSize - 1
	std::iota(mineSpots.begin(), mineSpots.end(), 0);

	// to shuffle this vector
	std::shuffle(mineSpots.begin(), mineSpots.end(), myRandomSeed);

	// to remove bad gridspots (those on and around chosen initial spot)
	mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
		(initChosenY * this->gridSize + initChosenX)), mineSpots.end());
	if (initChosenX > 0) {
		mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
			(initChosenY * this->gridSize + initChosenX - 1)), mineSpots.end());	//left
	}
	if (initChosenX < this->gridSize - 1) {
		mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
			(initChosenY * this->gridSize + initChosenX + 1)), mineSpots.end());	// right
	}
	if (initChosenY > 0) {
		mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
			((initChosenY - 1) * this->gridSize + initChosenX)), mineSpots.end());	// top
	}
	if (initChosenY < this->gridSize - 1) {
		mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
			((initChosenY + 1) * this->gridSize + initChosenX)), mineSpots.end());	// bottom
	}
	if (initChosenY > 0 && initChosenX > 0) {
		mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
			((initChosenY - 1) * this->gridSize + initChosenX - 1)), mineSpots.end());	// top left
	}
	if (initChosenY > 0 && initChosenX < this->gridSize - 1) {
		mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
			((initChosenY - 1) * this->gridSize + initChosenX + 1)), mineSpots.end());	// top right
	}
	if (initChosenY < this->gridSize - 1 && initChosenX > 0) {
		mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
			((initChosenY + 1) * this->gridSize + initChosenX - 1)), mineSpots.end());	// bottom left
	}
	if (initChosenY < this->gridSize - 1 && initChosenX < this->gridSize - 1) {
		mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
			((initChosenY + 1) * this->gridSize + initChosenX + 1)), mineSpots.end());	// bottom right
	}
}



void Grid::createMine(const int X, const int Y) {

	if (X < 0 || Y < 0 || X >= this->gridSize || Y >= this->gridSize) {
		std::cerr << "Error: Trying to create mine outside grid." << std::endl;
	}
	else {
		this->cells[Y][X]->putMine();
	}
}



void Grid::incrNumsAroundMine(const int X, const int Y) {

	if (X < 0 || Y < 0 || X >= this->gridSize || Y >= this->gridSize) {
		std::cerr << "Error: Trying to increment numbers around mine outside grid." << std::endl;
	}
	else {
		if (X > 0) {
			this->cells[Y][X - 1]->incrNumOfMinesAround();	//left
		}
		if (X < this->gridSize - 1) {
			this->cells[Y][X + 1]->incrNumOfMinesAround();	// right
		}
		if (Y > 0) {
			this->cells[Y - 1][X]->incrNumOfMinesAround();	// top
		}
		if (Y < this->gridSize - 1) {
			this->cells[Y + 1][X]->incrNumOfMinesAround();	// bottom
		}
		if (Y > 0 && X > 0) {
			this->cells[Y - 1][X - 1]->incrNumOfMinesAround();	// top left
		}
		if (Y > 0 && X < this->gridSize - 1) {
			this->cells[Y - 1][X + 1]->incrNumOfMinesAround();	// top right
		}
		if (Y < this->gridSize - 1 && X > 0) {
			this->cells[Y + 1][X - 1]->incrNumOfMinesAround();	// bottom left
		}
		if (Y < this->gridSize - 1 && X < this->gridSize - 1) {
			this->cells[Y + 1][X + 1]->incrNumOfMinesAround();	// bottom right
		}
	}
}



// to print the grid
void Grid::printGrid() const {
	std::cout << std::endl;
	this->printCharRow();
	for (int rowNum = 0; rowNum < this->gridSize; ++rowNum) {
		this->printLineRow();
		this->printRow(rowNum);
	}
	this->printLineRow();
	std::cout << std::endl;
}



// to print row of a grid
void Grid::printRow(const int rowNumber) const {
	std::cout << std::setw(3) << rowNumber + 1 << ' ';
	for (int i = 0; i < this->gridSize; ++i) {
		std::cout << "| " << this->cellPrintSymbol(i, rowNumber) << ' ';
	}
	std::cout << '|' << std::endl;
}



char Grid::cellPrintSymbol(const int X, const int Y) const {

	char symbol;
	if (X < 0 || Y < 0 || X >= this->gridSize || Y >= this->gridSize) {
		std::cerr << "Error: Trying to print cell outside grid." << std::endl;
		symbol = this->ERROR_SYMBOL;
	}
	else {
		if (this->cells[Y][X]->isMarked()) {
			symbol = this->MARKED_SYMBOL;
		}
		else {
			if (this->cells[Y][X]->isVisible()) {
				if (this->cells[Y][X]->hasMine()) {
					symbol = MINESYMBOL;
				}
				else {
					if (this->cells[Y][X]->numOfMinesAround() == 0) {
						symbol = this->NO_MINES_AROUND_SYMBOL;
					}
					else {
						symbol = static_cast<char>(this->cells[Y][X]->numOfMinesAround() + 48);
					}
				}
			}
			else {
				symbol = this->NOT_VISIBLE_SYMBOL;
			}
		}
	}
	return symbol;
}



// to print line between on a grid
void Grid::printLineRow() const {
	std::cout << "    ";
	for (int i = 0; i < this->gridSize; ++i) {
		std::cout << "+ - ";
	}
	std::cout << '+' << std::endl;
}



// to print letters above a grid
void Grid::printCharRow() const {

	// standard print for grids with size<27
	std::cout << "    ";
	if (this->gridSize <= 26) {
		for (int i = 0; i < this->gridSize; ++i) {
			std::cout << "  " << char(65 + i) << ' ';
		}
	}

	// additional print for grids size >=27 (works, but could possibly be prettier)
	if (this->gridSize > 26 && this->gridSize <= 675) {
		for (int i = 0; i < 26; ++i) {
			std::cout << "  " << char(65 + i) << ' ';
		}
		int charNumbOfLastEntry = this->gridSize / 26 - 1;
		for (int i = 0; i < charNumbOfLastEntry; ++i) {
			for (int j = 0; j < 26; ++j)
				std::cout << "  " << char(65 + i) << char(65 + j);
		}
		for (int i = 0; i < this->gridSize % 26; ++i) {
			std::cout << "  " << char(65 + charNumbOfLastEntry) << char(65 + i);
		}
	}

	std::cout << ' ' << std::endl;
}



// to print out the solution 
//	* condensed layout
//	* should probably be redone
//	* might not work with large grids
void Grid::printSolution() const {

	std::cout << "\nHere is the solution:\n" << std::endl;

	std::cout << std::setw(5);
	for (int i = 0; i < this->gridSize; ++i) {
		std::cout << ' ' << char(65 + i);
	}
	std::cout << std::setw(10);
	for (int i = 0; i < this->gridSize; ++i) {
		std::cout << ' ' << char(65 + i);
	}
	std::cout << std::endl;

	for (int i = 0; i < this->gridSize; ++i) {
		std::cout << std::setw(3) << i + 1 << '|';
		for (int j = 0; j < this->gridSize; ++j) {
			if (this->cells[i][j]->hasMine()) {
				std::cout << ' ' << this->MINESYMBOL;
			}
			else if (this->cells[i][j]->numOfMinesAround() == 0) {
				std::cout << ' ' << this->NO_MINES_AROUND_SYMBOL;
			}
			else {
				std::cout << ' ' << this->cells[i][j]->numOfMinesAround();
			}
		}

		// simplified solution grid (shows only mines)
		std::cout << std::setw(8) << i + 1 << '|';
		for (int j = 0; j < this->gridSize; ++j) {
			if (this->cells[i][j]->hasMine()) {
				std::cout << ' ' << '*';
			}
			else {
				std::cout << ' ' << 0;
			}
		}
		std::cout << std::endl;
	}
	std::cout << "\nPress ENTER to continue..." << std::flush;
	std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
}



// to mark (or unmark) given coordinates, and keeping track of marked and wrongly marked mines
void Grid::markInputCoordinates(const int X, const int Y) {

	if (X < 0 || Y < 0 || X >= this->gridSize || Y >= this->gridSize) {
		std::cerr << "Error: Trying to mark cell outside grid." << std::endl;
	}
	else if (this->cells[Y][X]->isMarked()) {
		this->cells[Y][X]->unmarkCell();

		if (this->cells[Y][X]->hasMine()) {
			--(this->numOfMarkedMines);
		}
		else {
			--(this->numOfWronglyMarkedMines);
		}
	}
	else {
		this->cells[Y][X]->markCell();

		if (this->cells[Y][X]->hasMine()) {
			++(this->numOfMarkedMines);
		}
		else {
			++(this->numOfWronglyMarkedMines);
		}
	}
}



bool Grid::allMinesMarked() const {

	if (this->numOfMines == this->numOfMarkedMines && this->numOfWronglyMarkedMines == 0) {
		return true;
	}
	else {
		return false;
	}
}



// to check user given coordinates, and make it visible
bool Grid::checkInputCoordinates(const int X, const int Y) {

	bool checkedMine = false;
	if (X < 0 || Y < 0 || X >= this->gridSize || Y >= this->gridSize) {
		std::cerr << "Error: Trying to check cell outside grid." << std::endl;
	}
	else if (!(this->cells[Y][X]->isVisible()) && !(this->cells[Y][X]->isMarked())) {
		this->cells[Y][X]->makeVisible();

		if (this->cells[Y][X]->hasMine()) {
			checkedMine = true;
		}
		else if (this->cells[Y][X]->numOfMinesAround() == 0) {
			checkedMine = this->checkAroundCoordinate(X, Y);
		}
	}
	return checkedMine;
}



bool Grid::checkAroundCoordinate(const int X, const int Y) {

	bool checkedMine = false;
	if (X > 0) {
		if (this->checkInputCoordinates(X - 1, Y)) {	// left
			checkedMine = true;
		}
	}
	if (X < this->gridSize - 1) {
		if (this->checkInputCoordinates(X + 1, Y)) {		// right
			checkedMine = true;
		}
	}
	if (Y > 0) {
		if (this->checkInputCoordinates(X, Y - 1)) {		// top
			checkedMine = true;
		}
	}
	if (Y < this->gridSize - 1) {
		if (this->checkInputCoordinates(X, Y + 1)) {		// bottom
			checkedMine = true;
		}
	}
	if (X > 0 && Y > 0) {
		if (this->checkInputCoordinates(X - 1, Y - 1)) {	// top left
			checkedMine = true;
		}
	}
	if (Y > 0 && X < this->gridSize - 1) {
		if (this->checkInputCoordinates(X + 1, Y - 1)) {	// top right
			checkedMine = true;
		}
	}
	if (Y < this->gridSize - 1 && X > 0) {
		if (this->checkInputCoordinates(X - 1, Y + 1)) {	// bottom left
			checkedMine = true;
		}
	}
	if (X < this->gridSize - 1 && Y < this->gridSize - 1) {
		if (this->checkInputCoordinates(X + 1, Y + 1)) {	// bottom right
			checkedMine = true;
		}
	}
	return checkedMine;
}