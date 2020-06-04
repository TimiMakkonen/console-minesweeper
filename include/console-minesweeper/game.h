#ifndef CONSOLE_MINESWEEPER_GAME_H
#define CONSOLE_MINESWEEPER_GAME_H

#include <string>
#include <memory>

#include <minesweeper/i_random.h>
#include <minesweeper/game.h>

namespace console_minesweeper {

	class Game {
	private:

		const int MAXIMUM_GRID_HEIGHT = 99; // DO NOT CHANGE!
		const int MINIMUM_GRID_HEIGHT = 4; // DO NOT CHANGE!
		const int MAXIMUM_GRID_WIDTH = MAXIMUM_GRID_HEIGHT; // DO NOT CHANGE!
		const int MINIMUM_GRID_WIDTH = MINIMUM_GRID_HEIGHT; // DO NOT CHANGE!

		// symbols for visuals
		const char MINESYMBOL = '*';
		const char MARKED_SYMBOL = '#'; // 'M' could also work
		const char NO_MINES_AROUND_SYMBOL = char(254);
		const char NOT_VISIBLE_SYMBOL = ' ';
		const char ERROR_SYMBOL = 'E';

		minesweeper::IRandom* random;
		int gridWidth;
		int gridHeight;
		int numOfMines;
		std::unique_ptr<minesweeper::Game> currentGame = nullptr;

		enum class Options {
			QUIT,
			NOTHING,
			SOLUTION,
			HELP
		};

		void welcomeText() const;

		void chooseGridSize();

		int inputStringToInt(std::string input);

		void chooseNumOfMines(bool firstTry);

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

		Game(minesweeper::IRandom* random);

		void run();

	};

}

#endif // CONSOLE_MINESWEEPER_GAME_H