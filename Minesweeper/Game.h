#ifndef GAME_H
#define GAME_H

#include <string>
#include <memory>

#include "Grid.h"

namespace Minesweeper {

	class Game {
	private:

		const int MAXIMUM_GRID_SIZE = 99;
		const int MINIMUM_GRID_SIZE = 4;

		// symbols for visuals
		const char MINESYMBOL = '*';
		const char MARKED_SYMBOL = '#'; // 'M' could also work
		const char NO_MINES_AROUND_SYMBOL = char(254);
		const char NOT_VISIBLE_SYMBOL = ' ';
		const char ERROR_SYMBOL = 'E';

		int gridSize;
		int numOfMines;
		std::unique_ptr<Grid> currentGrid = nullptr;

		enum class Options {
			QUIT,
			NOTHING,
			SOLUTION,
			HELP
		};

		void welcomeText() const;

		void chooseGridSize();

		void chooseNumOfMines();

		void initEmptyGrid();

		void helpText() const;

		// to check for special input options: solution, quit and help
		Options checkForOptions(const std::string input, bool gridHasBeenCreated) const;

		// to transfer input string of coordinates into coordinates for the program (return false if not able to do this)
		bool inputToCoordinates(std::string userInput, int& chosenX, int& chosenY, bool& wantToMarkInput) const;

		// to transfer intepreted coordinates back to input letters (to let user know, what the program thought they meant)
		std::string xCoordToLetters(const int chosenX) const;

		void winScreen() const;

		void gameOverScreen() const;

		std::pair<bool, bool> wantToPlayAgain() const;

		bool userSaysYes(std::string userInput) const;

		char cellPrintSymbol(const int X, const int Y) const;

		void printCharRow() const;

		void printLineRow() const;

		void printRow(const int RowNumber) const;

		void printGrid() const;

		void printSolution() const;

	public:

		void run();

	};

}

#endif