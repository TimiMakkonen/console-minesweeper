#include <memory>
#include <iostream>
#include <string>
#include <algorithm>


#include "Game.h"
#include "Grid.h"



void Game::run() const {

	// variable to determine if you want to play again
	bool wantToPlayAgain = false;

	do {

		this->welcomeText();

		int gridSize = this->chooseGridSize();

		int numOfMines = this->chooseNumOfMines(gridSize);


		// variable to determine if you want to play again with the same rules
		bool wantToPlayAgainSameRules = false;

		do {

			std::unique_ptr<Grid> currentGrid = std::make_unique<Grid>(gridSize, numOfMines);

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
			int chosenOptionNum = 0;
			for (int gameAutoTimeout = 0; gameAutoTimeout <= gridSize * gridSize + 100; ++gameAutoTimeout) {

				if (wantToSeeGrid) {
					currentGrid->printGrid();
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
				// -1: QUIT, 1:SOLUTION, 2:HELP
				chosenOptionNum = checkForOptions(currentGrid, userInput, gridHasBeenCreated);
				if (chosenOptionNum == 1) {
					wantToSeeGrid = true;
					continue;
				}
				else if (chosenOptionNum == 2) {
					wantHelp = true;
					std::cout << std::endl;
					continue;
				}
				else if (chosenOptionNum == -1) {
					break;
				}


				// Checking which spot user picked and if they want to check or mark it
				wantToMarkInput = false; // wantToMarkInput will change to true if user wants to mark/unmark some cell
				if (!inputToCoordinates(userInput, chosenX, chosenY, wantToMarkInput)) {
					std::cout << "Please try to choose a grid spot again to mark or check: "
									"(Type 'HELP' for instructions.)" << std::endl;
					wantToSeeDefaultInputQ = false;
					continue;
				}


				// to check that the intepreted gridspot exists (is withing range)
				if (chosenX < 0 || chosenX  > gridSize - 1 || chosenY < 0 || chosenY  > gridSize - 1) {
					std::cout << "Chosen coordinates out of range! Please choose a grid spot again to mark or check: "
									"(Type 'HELP' for instructions.)" << std::endl;
					wantToSeeDefaultInputQ = false;
					continue;
				}



				// create the grid after user has chosen the first coordinates to guarantee a good start
				if (!gridHasBeenCreated) {
					currentGrid->createMinesAndNums(chosenX, chosenY);
					gridHasBeenCreated = true;
				}



				// to update visible grid by checking or (un)marking
				if (!wantToMarkInput) {

					bool checkedMine = currentGrid->checkInputCoordinates(chosenX, chosenY);
					currentGrid->printGrid();

					std::cout << "You chose to check: "
							  << xCoordToLetters(chosenX) << ' ' << chosenY + 1 << '\n' << std::endl;

					// lost because of checking a mine
					if (checkedMine) {
						std::cout << "\n\n**********You Lost!**********\n\n"
								  << "\nPress ENTER to see the solution..." << std::flush;
						std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
						currentGrid->printSolution();
						std::cout << "\n\n**********You Lost!**********\n" << std::endl;
						break;
					}
				}
				else {
					
					currentGrid->markInputCoordinates(chosenX, chosenY);

					currentGrid->printGrid();

					std::cout << "You chose to mark: "
							  << xCoordToLetters(chosenX) << ' ' << chosenY + 1 << '\n' << std::endl;

					// Win condition
					if (currentGrid->allMinesMarked()) {
						std::cout << "\n\n**********You Won!**********\n\n"
								  << "\nPress ENTER to see the solution..." << std::flush;
						std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
						currentGrid->printSolution();
						std::cout << "\n\n**********You Won!**********\n" << std::endl;
						break;
					}
				}
			}



			std::cout << "Thank you for playing!\n"
					  << "Would you like to play again? (Y/N)" << std::endl;
			std::string wantToPlayAgainStr;
			getline(std::cin, wantToPlayAgainStr);
			std::cout << std::endl;

			this->userSaysYes(wantToPlayAgainStr);

			if (this->userSaysYes(wantToPlayAgainStr)) {

				std::cout << "Would you like to use the same rule set? (Size of the grid: " 
						  << gridSize << 'x' << gridSize << " and the number of mines: " 
						  << numOfMines << ") (Y/N)" << std::endl;
				std::string wantToUseSameRules;
				getline(std::cin, wantToUseSameRules);
				std::cout << "------------------------------------------" << std::endl;
				if (this->userSaysYes(wantToUseSameRules)) {
					wantToPlayAgainSameRules = true;
				}
				else {
					wantToPlayAgain = true;
				}
			}
		} while (wantToPlayAgainSameRules);

	} while (wantToPlayAgain);

	std::cout << "Good bye!" << std::endl;

}



void Game::welcomeText() const {
	
	std::cout << "Welcome to basic Minesweeper game!\n"
			  << "This program does NOT provide you with the rules of Minesweeper.\n"
			  << "To find out the rules of Minesweeper, you can rely on the information available online.\n\n"
			  << "Good luck and have fun!\n" << std::endl;
}



int Game::chooseGridSize() const {

	std::cout << "First of all, choose desired grid size: (I recommend '10' => 10x10 grid)" << std::endl;

	// input grid size
	std::string gridSizeStr;
	bool validSizeInput = false;
	int gridSize;
	while (!validSizeInput) {
		try {
			getline(std::cin, gridSizeStr);
			gridSize = stoi(gridSizeStr); // if input (gridSizeStr) is not a number, throws exception
			if (gridSize > this->MAXIMUM_GRID_SIZE || gridSize <= 0) {
				std::cout << "Too small or large number! Choose again: (Choose a whole number between 1 and "
						  << this->MAXIMUM_GRID_SIZE << ". I recommend number to be <50. )" << std::endl;
			}
			else {
				validSizeInput = true;
			}
		}
		catch (...) {
			std::cout << "Not a number! Please try to enter the grid size again:" << std::endl;
		}
	}
	return gridSize;
}



int Game::chooseNumOfMines(const int gridSize) const {

	std::cout << "Perfect! Now choose the number of mines:" << std::endl;

	// input to number of mines
	std::string numOfMinesStr;
	bool validNumOfMinesInput = false;
	int numOfMines;
	while (!validNumOfMinesInput) {
		try {
			getline(std::cin, numOfMinesStr);
			numOfMines = stoi(numOfMinesStr); // if input (numOfMinesStr) is not a number, throws exception
			if (numOfMines > gridSize * gridSize - 9 || numOfMines <= 0) {
				std::cout << "Too many or not enough mines! Choose again: (Choose a whole number between 1 and "
						  << gridSize * gridSize - 9 << " )" << std::endl;
			}
			else {
				validNumOfMinesInput = true;
			}
		}
		catch (...) {
			std::cout << "Not a number! Please try enter the number of mines again:" << std::endl;
		}
	}
	return numOfMines;
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
		std::cerr << "Error: Coordinate to Letters conversion failed!" << std::endl;
		return "ERROR";
	}
}



// to check for special input options: solution, quit and help
int Game::checkForOptions(std::unique_ptr<Grid> const& currentGrid, std::string input, const bool gridHasBeenCreated) const {

	std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return std::toupper(c); });

	// quit program and give solution, if user asks for it
	if (input == "QUIT" || input == "Q") {
		std::cout << "\nI am sorry that you have to leave." << std::endl;
		if (gridHasBeenCreated) {
			currentGrid->printSolution();
		}
		std::cout << std::endl;
		return -1;
	}

	// user wants to see the help text
	if (input == "HELP" || input == "H" || input == "INFO" || input == "INSTRUCTIONS" || input == "I") {
		return 2;
	}

	// solution given, if user asks for it
	if (input == "SOLUTION" || input == "SOL" || input == "S") {
		if (gridHasBeenCreated) {
			currentGrid->printSolution();
		}
		else {
			std::cout << "\nThe grid will be created after the first move. Hence solution does not exist, yet.\n"
					  << "\nPress ENTER to continue..." << std::flush;
			std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
		}
		std::cout << std::endl;
		return 1;
	}

	return 0;
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