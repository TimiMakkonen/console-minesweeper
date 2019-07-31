#ifndef GAME_H
#define GAME_H

#include <string>
#include <memory>

#include "Grid.h"

class Game {
private:

	int MAXIMUM_GRID_SIZE = 99;

	enum class Options {
		QUIT,
		NOTHING,
		SOLUTION,
		HELP
	};


	void welcomeText() const;

	int chooseGridSize() const;

	int chooseNumOfMines(const int gridSize) const;

	void helpText() const;
	
	// to check for special input options: solution, quit and help
	Options checkForOptions(std::unique_ptr<Grid> const& currentGrid, const std::string input, bool gridHasBeenCreated) const;

	// to transfer input string of coordinates into coordinates for the program (return false if not able to do this)
	bool inputToCoordinates(std::string userInput, int& chosenX, int& chosenY, bool& wantToMarkInput) const;

	// to transfer intepreted coordinates back to input letters (to let user know, what the program thought they meant)
	std::string xCoordToLetters(const int chosenX) const;

	void winScreen(std::unique_ptr<Grid> const& currentGrid) const;

	void gameOverScreen(std::unique_ptr<Grid> const& currentGrid) const;

	bool userSaysYes(std::string userInput) const;

public:
	
	void run() const;

};

#endif