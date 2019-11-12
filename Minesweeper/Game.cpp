#include <memory>
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

#include "Game.h"
#include "Grid.h"

namespace Minesweeper {

	void Game::run() {

		// variable to determine if you want to play again
		bool playerWantsToPlayAgain = false;

		do {

			this->welcomeText();

			// ask user for gridSize
			this->chooseGridSize();

			// ask user for numOfMines
			this->chooseNumOfMines();


			// variable to determine if you want to play again with the same rules
			bool playerWantsToPlayAgainSame = false;

			do {

				initEmptyGrid();

				// The Start Of The Game Itself
				std::cout << "\nSplendid! Enjoy your game!" << std::endl;
				std::string userInput;
				bool gridHasBeenCreated = false;
				bool wantToSeeGrid = true;
				bool wantToSeeDefaultInputQ = true;
				bool wantHelp = true;
				bool wantToMarkInput;
				int chosenX;
				int chosenY;
				Options chosenOption;
				for (int gameAutoTimeout = 0; gameAutoTimeout <= this->gridSize * this->gridSize + 100; ++gameAutoTimeout) {

					if (wantToSeeGrid) {
						this->printGrid();
						wantToSeeGrid = false;
					}

					if (wantToSeeDefaultInputQ) {
						if (wantHelp) {
							this->helpText();
						}
						else {
							std::cout << "(Type 'HELP' for instructions.)" << std::endl;
						}
						std::cout << "Choose a grid spot to mark or check:" << std::endl;
					}
					wantToSeeDefaultInputQ = true;
					wantHelp = false;

					getline(std::cin, userInput);

					// to check if we received special instructions
					chosenOption = checkForOptions(userInput, gridHasBeenCreated);
					if (chosenOption == Options::SOLUTION) {
						wantToSeeGrid = true;
						continue;
					}
					else if (chosenOption == Options::HELP) {
						wantHelp = true;
						std::cout << std::endl;
						continue;
					}
					else if (chosenOption == Options::QUIT) {
						break;
					}


					// Checking which spot user picked and if they want to check or mark it
					wantToMarkInput = false;
					if (!inputToCoordinates(userInput, chosenX, chosenY, wantToMarkInput)) {
						std::cout << "Please try to choose a grid spot again to mark or check: "
							"(Type 'HELP' for instructions.)" << std::endl;
						wantToSeeDefaultInputQ = false;
						continue;
					}


					// to check that the intepreted gridspot exists (within range)
					if (chosenX < 0 || chosenX  > this->gridSize - 1 || chosenY < 0 || chosenY  > this->gridSize - 1) {
						std::cout << "Chosen coordinates out of range! Please choose a grid spot again to mark or check: "
							"(Type 'HELP' for instructions.)" << std::endl;
						wantToSeeDefaultInputQ = false;
						continue;
					}



					// create the grid after user has chosen the first coordinates to guarantee a good start
					if (!gridHasBeenCreated) {
						this->currentGrid->createMinesAndNums(chosenX, chosenY);
						gridHasBeenCreated = true;

						if (wantToMarkInput) {
							std::cout << "\nYou wanted to mark an input on your first turn.\n"
								<< "Unfortunately I am not going to let you do this. ;)" << std::endl;
							wantToMarkInput = false;
						}
					}



					// to update visible grid by checking or (un)marking
					if (wantToMarkInput) {

						this->currentGrid->markInputCoordinates(chosenX, chosenY);

						this->printGrid();

						std::cout << "You chose to mark: "
							<< this->xCoordToLetters(chosenX) << ' ' << chosenY + 1 << '\n' << std::endl;
					}
					else {

						this->currentGrid->checkInputCoordinates(chosenX, chosenY);

						this->printGrid();

						std::cout << "You chose to check: "
							<< xCoordToLetters(chosenX) << ' ' << chosenY + 1 << '\n' << std::endl;

						if (this->currentGrid->playerHasLost()) {
							// Losing condition: checked a mine
							this->gameOverScreen();
							break;
						}
					}

					if (this->currentGrid->playerHasWon()) {
						// Win condition: all non mines visible/checked or all mines marked
						this->winScreen();
						break;
					}
				}//endfor
				

				std::tie(playerWantsToPlayAgain, playerWantsToPlayAgainSame) = this->wantToPlayAgain();


			} while (playerWantsToPlayAgainSame);

		} while (playerWantsToPlayAgain);

		std::cout << "Good bye!" << std::endl;

	}



	void Game::welcomeText() const {

		std::cout << "Welcome to basic Minesweeper game!\n"
			<< "This program does NOT provide you with the rules of Minesweeper.\n"
			<< "To find out the rules of Minesweeper, you can rely on the information available online.\n\n"
			<< "Good luck and have fun!\n" << std::endl;
	}



	void Game::chooseGridSize() {

		std::cout << "First of all, choose desired grid size: (I recommend '10' => 10x10 grid)" << std::endl;

		// input grid size
		std::string gridSizeStr;
		bool validSizeInput = false;
		while (!validSizeInput) {
			try {
				getline(std::cin, gridSizeStr);
				this->gridSize = stoi(gridSizeStr); // if input (gridSizeStr) is not a number, throws exception
				if (this->gridSize > this->MAXIMUM_GRID_SIZE || this->gridSize < MINIMUM_GRID_SIZE) {
					std::cout << "Too small or large number! Choose again: (Choose a whole number between "
						<< this->MINIMUM_GRID_SIZE << " and "
						<< this->MAXIMUM_GRID_SIZE << ". I recommend number to be <50. )" << std::endl;
				}
				else {
					validSizeInput = true;
				}
			}
			catch (const std::invalid_argument&) {
				std::cout << "Not a number! Please try to enter the grid size again:" << std::endl;
			}
			catch (const std::out_of_range&) {
				std::cout << "Too large number! Please try to enter the grid size again: (Choose a whole number between 1 and "
					<< this->gridSize * this->gridSize - 9 << " )" << std::endl;
			}
		}
	}



	void Game::chooseNumOfMines() {

		std::cout << "Perfect! Now choose the number of mines:" << std::endl;

		// input to number of mines
		std::string numOfMinesStr;
		bool validNumOfMinesInput = false;
		while (!validNumOfMinesInput) {
			try {
				getline(std::cin, numOfMinesStr);
				this->numOfMines = stoi(numOfMinesStr); // if input (numOfMinesStr) is not a number, throws exception
				if (this->numOfMines > this->gridSize * this->gridSize - 9 || this->numOfMines <= 0) {
					std::cout << "Too many or not enough mines! Choose again: (Choose a whole number between 1 and "
						<< this->gridSize * this->gridSize - 9 << " )" << std::endl;
				}
				else {
					validNumOfMinesInput = true;
				}
			}
			catch (const std::invalid_argument&) {
				std::cout << "Not a number! Please try enter the number of mines again:" << std::endl;
			}
			catch (const std::out_of_range&) {
				std::cout << "Too large number! Please try enter the number of mines again: (Choose a whole number between 1 and "
					<< this->gridSize * this->gridSize - 9 << " )" << std::endl;
			}
		}
	}

	void Game::initEmptyGrid() {

		currentGrid = std::make_unique<Grid>(this->gridSize, this->numOfMines);
	}

	void Game::helpText() const {

		std::cout << "\n\nTo check a grid spot type: 'A5', 'B2, etc.\n"
			<< "To mark a grid spot, type: 'A2 M', 'C6 M', etc.\n"
			<< "These inputs are NOT case NOR space sensitive. For example typing 'b3m' also marks B3.\n"
			<< "You can type 'QUIT' to quit the game or 'SOL' to see the solution.\n"
			<< "You can type 'HELP' to see this message again." << std::endl;
	}



	// to transfer intepreted coordinates back to input letters (to let user know, what the program thought they meant)
	std::string Game::xCoordToLetters(const int chosenX) const {
		if (chosenX <= 25) {
			std::string coordToLetter = "A";
			coordToLetter = (chosenX + 65);
			return coordToLetter;
		}
		else if (chosenX > 25 && chosenX < 675) {
			std::string coordToLetter = "AA";
			coordToLetter[0] = char((chosenX / 26) + 64);
			coordToLetter[1] = char((chosenX % 26) + 65);
			return coordToLetter;
		}
		else {
			throw std::invalid_argument("Game::xCoordToLetters(const int chosenX): Coordinate to Letters conversion failed.");
		}
	}

	void Game::winScreen() const {

		std::cout << "\n\n**********You Won!**********\n\n"
			<< "\nPress ENTER to see the solution..." << std::flush;
		std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
		this->printSolution();
		std::cout << "\n\n**********You Won!**********\n" << std::endl;
	}

	void Game::gameOverScreen() const {

		std::cout << "\n\n**********You Lost!**********\n\n"
			<< "\nPress ENTER to see the solution..." << std::flush;
		std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
		this->printSolution();
		std::cout << "\n\n**********You Lost!**********\n" << std::endl;
	}

	std::pair<bool, bool> Game::wantToPlayAgain() const {

		std::cout << "Thank you for playing!\n"
			<< "Would you like to play again? (Y/N)" << std::endl;
		std::string wantToPlayAgainStr;
		getline(std::cin, wantToPlayAgainStr);
		std::cout << std::endl;

		bool playerWantsToPlayAgain = this->userSaysYes(wantToPlayAgainStr);
		bool wantToPlayAgainSameRules = false;

		if (playerWantsToPlayAgain) {

			std::cout << "Would you like to use the same rule set? (Size of the grid: "
				<< this->gridSize << 'x' << this->gridSize << " and the number of mines: "
				<< this->numOfMines << ") (Y/N)" << std::endl;
			std::string wantToUseSameRules;
			getline(std::cin, wantToUseSameRules);
			std::cout << "------------------------------------------" << std::endl;
			if (this->userSaysYes(wantToUseSameRules)) {
				wantToPlayAgainSameRules = true;
			}
		}

		return std::pair<bool, bool>(playerWantsToPlayAgain, wantToPlayAgainSameRules);
	}



	// to check for special input options: solution, quit and help
	Game::Options Game::checkForOptions(std::string input, const bool gridHasBeenCreated) const {

		std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return std::toupper(c); });

		// quit program and give solution, if user asks for it
		if (input == "QUIT" || input == "Q") {
			std::cout << "\nI am sorry that you have to leave." << std::endl;
			if (gridHasBeenCreated) {
				this->printSolution();
			}
			std::cout << std::endl;
			return Options::QUIT;
		}

		// user wants to see the help text
		if (input == "HELP" || input == "H" || input == "INFO" || input == "INSTRUCTIONS" || input == "I") {
			return Options::HELP;
		}

		// solution given, if user asks for it
		if (input == "SOLUTION" || input == "SOL" || input == "S") {
			if (gridHasBeenCreated) {
				this->printSolution();
			}
			else {
				std::cout << "\nThe grid will be created after the first move. Hence solution does not exist, yet.\n"
					<< "\nPress ENTER to continue..." << std::flush;
				std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
			}
			std::cout << std::endl;
			return Options::SOLUTION;
		}

		return Options::NOTHING;
	}



	// to transfer input string of coordinates into coordinates for the program (return false if not able to do this)
	bool Game::inputToCoordinates(std::string userInput, int& chosenX, int& chosenY, bool& wantToMarkInput) const {

		int inputLength = userInput.length();


		// to capitalise input
		std::transform(userInput.begin(), userInput.end(), userInput.begin(),
			[](unsigned char c) { return std::toupper(c); });


		// to remove spaces
		while (userInput.find(' ') != std::string::npos) {
			userInput = userInput.erase(userInput.find(' '), 1);
		}


		// to make sure the first character is a letter
		int i = 0;
		if (int(userInput[i]) < 65 || int(userInput[i]) > 90) {
			std::cout << "\nFirst character not a letter!" << std::endl;
			return false;
		}

		// to find when letters turn into numbers
		while (int(userInput[i]) >= 65 && int(userInput[i]) <= 90) {
			++i;

			// to make sure input doesn't have only letters (hence options should be checked before this function)
			if (inputLength == i) {
				std::cout << "\nIncorrect input! (input contained only letters, no numbers)" << std::endl;
				return false;
			}
		}

		int indexOfFirstNumber = i; // first non-letter (should be a number)
		if (int(userInput[indexOfFirstNumber]) < 48 || int(userInput[indexOfFirstNumber]) > 57) {
			std::cout << "\nFirst non-character not a number!" << std::endl;
			return false;
		}


		// Letter(s) to find the first coordinate
		if (indexOfFirstNumber == 1) {
			chosenX = int(userInput[0]) - 65;
		}
		else if (indexOfFirstNumber == 2) {
			chosenX = (int(userInput[0]) - 64) * 26 + (int(userInput[1]) - 65);
		}
		else {
			std::cout << "\nCoordinate corresponding to the given column letters too large!" << std::endl;
			return false;
		}


		// to find when numbers stop
		while (int(userInput[i]) >= 48 && int(userInput[i]) <= 57) {
			++i;
			// to make sure we don't try to access non-existent position
			if (inputLength == i) break;
		}


		// Numbers to find the second coordinate
		int indexOfLastNumber = i - 1;
		chosenY = stoi(userInput.substr(indexOfFirstNumber, indexOfLastNumber - indexOfFirstNumber + 1)) - 1;


		// index of M is 'i', if it exists
		if (inputLength - 1 >= i) {
			if (userInput[i] == 'M') {
				wantToMarkInput = true;
			}
		}
		return true;
	}



	bool Game::userSaysYes(std::string userInput) const {

		std::transform(userInput.begin(), userInput.end(), userInput.begin(),
			[](unsigned char c) { return std::toupper(c); });

		if (userInput == "Y" || userInput == "YES" || userInput == "1") {
			return true;
		}
		else {
			return false;
		}
	}








	// TODO CHECKED
	char Game::cellPrintSymbol(const int X, const int Y) const {

		char symbol;
		if (X < 0 || Y < 0 || X >= this->gridSize || Y >= this->gridSize) {
			throw std::out_of_range("Game::cellPrintSymbol(const int X, const int Y): Trying to print cell outside grid.");
		}
		else {
			if (currentGrid->isCellMarked(X, Y)) {
				symbol = this->MARKED_SYMBOL;
			}
			else {
				if (currentGrid->isCellVisible(X, Y)) {
					if (currentGrid->doesCellHaveMine(X, Y)) {
						symbol = MINESYMBOL;
					}
					else {
						if (currentGrid->numOfMinesAroundCell(X, Y) == 0) {
							symbol = this->NO_MINES_AROUND_SYMBOL;
						}
						else {
							symbol = static_cast<char>(currentGrid->numOfMinesAroundCell(X, Y) + 48);
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

	// TODO CHECKED
	// to print letters above a grid
	void Game::printCharRow() const {

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

	// TODO CHECKED
	// to print lines between on a grid
	void Game::printLineRow() const {
		std::cout << "    ";
		for (int i = 0; i < this->gridSize; ++i) {
			std::cout << "+ - ";
		}
		std::cout << '+' << std::endl;
	}

	// TODO CHECKED
	void Game::printRow(const int rowNumber) const {
		std::cout << std::setw(3) << rowNumber + 1 << ' ';
		for (int i = 0; i < this->gridSize; ++i) {
			std::cout << "| " << this->cellPrintSymbol(i, rowNumber) << ' ';
		}
		std::cout << '|' << std::endl;
	}

	// TODO CHECKED
	void Game::printGrid() const {
		std::cout << std::endl;
		this->printCharRow();
		for (int rowNum = 0; rowNum < this->gridSize; ++rowNum) {
			this->printLineRow();
			this->printRow(rowNum);
		}
		this->printLineRow();
		std::cout << std::endl;
	}

	// TODO CHECKED
	// to print out the solution 
	//	* condensed layout
	//	* should probably be redone
	//	* does not properly work with large grids (for now)
	void Game::printSolution() const {

		std::cout << "\nHere is the solution:\n" << std::endl;

		// to print letters above solution grid
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
				if (currentGrid->doesCellHaveMine(j, i)) {
					std::cout << ' ' << this->MINESYMBOL;
				}
				else if (currentGrid->numOfMinesAroundCell(j, i) == 0) {
					std::cout << ' ' << this->NO_MINES_AROUND_SYMBOL;
				}
				else {
					std::cout << ' ' << currentGrid->numOfMinesAroundCell(j, i);
				}
			}

			// simplified solution grid (shows only mines)
			std::cout << std::setw(8) << i + 1 << '|';
			for (int j = 0; j < this->gridSize; ++j) {
				if (currentGrid->doesCellHaveMine(j, i)) {
					std::cout << ' ' << this->MINESYMBOL;
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

}