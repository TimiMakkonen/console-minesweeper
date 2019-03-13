#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::setw;
using std::vector;
using std::string;


// symbols for visuals
const char MineSymbol = '*';
const char MarkedMineSymbol = '#'; // 'M'
const char NoMinesAroundSymbol = char(254);
const char NotChosenSymbol = ' ';






//--------------------------------------------------------------------------------
//									Declarations:
//--------------------------------------------------------------------------------






// random generator function to use with std::random_shuffle
int MyRandomGen(int i);

// to print row of a grid
void PrintRow(vector< vector<char> > &VisibleGrid, int GridSize, int RowNumber);

// to print line between on a grid
void PrintLineRow(int GridSize);

// to print letters above a grid
void PrintCharRow(int GridSize);

// to print the grid
void PrintGrid(vector< vector<char> > &VisibleGrid, int GridSize);

// to create the data grid with mines
void CreateGrid(vector< vector<int> > &Grid, int GridSize, int NumberOfMines, int(&ChosenCoordinates)[2]);

// to transfer input string of coordinates into coordinates for the program (return false if not able to do this)
bool TryInputToCoordinates(string ChosenSpot, int(&ChosenCoordinates)[2], bool &MarkInput);

// to transfer intepreted coordinates back to input letters (to let user know, what the program thought they meant)
string Coordinate0ToLetters(int ChosenCoordinate0);

// to check user given coordinates, and update result
void CheckInputCoordinates(vector< vector<int> > &Grid, vector< vector<char> > &VisibleGrid, int ChosenCoordinate0, int ChosenCoordinate1);

// to check around given coordinate
void CheckAroundCoordinate(vector< vector<int> > &Grid, vector< vector<char> > &VisibleGrid, int ChosenCoordinate0, int ChosenCoordinate1);

// to mark (or unmark) user given coordinates, and update visual result and keeping track of winning progress
void MarkInputCoordinates(int GridSpotToDataForMark, vector< vector<char> > &VisibleGrid, int(&ChosenCoordinates)[2], int &NumberOfMarkedMines, int &NumberOfWronglyMarkedMines);

// to print out the solution
void Solution(vector< vector<int> > Grid, int GridSize);

// to check for special input options: solution, quit and help
int CheckForOptions(vector< vector<int> > &Grid, int GridSize, string Input, bool GridHasBeenCreated);






//--------------------------------------------------------------------------------
//										Main:
//--------------------------------------------------------------------------------








int main() {

	// to set random seed
	srand(time(NULL));



	// variable to determine if you want to play again
	bool PlayAgain = false;

	do {

		// Welcome text
		cout << "Welcome to basic Minesweeper game!" << endl;
		cout << "This program does NOT provide you with the rules of Minesweeper." << endl;
		cout << "To find out the rules of Minesweeper, you can rely on the information available online." << endl << endl;
		cout << "Good luck and have fun!" << endl << endl;
		cout << "First of all, choose desired grid size: (I recommend '10' => 10x10 grid) " << endl;


		// Maximum Grid Size
		int MaximumGridSize = 99;


		// Input grid size
		string GridSizeStr;
		bool ValidSizeInput = false;
		int GridSize = 0;
		while (!ValidSizeInput) {
			try {
				getline(cin, GridSizeStr);
				GridSize = stoi(GridSizeStr); // if input (GridSizeStr) is not a number, throws exception
				if (GridSize > MaximumGridSize || GridSize <= 0) {
					cout << "Too small or large number! Choose again: (Choose a whole number between 1 and " << MaximumGridSize << ". I recommend number <50. )" << endl;
				}
				else {
					ValidSizeInput = true;
				}
			}
			catch (...) {
				cout << "Not a number! Please try to enter the grid size again: " << endl;
			}
		}


		cout << "Perfect! Now choose the number of mines: " << endl;


		// Input number of mines
		string NumberOfMinesStr;
		bool ValidNumOfMinesInput = false;
		int NumberOfMines = 0;
		while (!ValidNumOfMinesInput) {
			try {
				getline(cin, NumberOfMinesStr);
				NumberOfMines = stoi(NumberOfMinesStr); // if input (NumberOfMinesStr) is not a number, throws exception
				if (NumberOfMines > GridSize * GridSize - 9 || NumberOfMines <= 0) {
					cout << "Too many or not enough mines! Choose again: (Choose a whole number between 1 and " << GridSize * GridSize - 9 << " )" << endl;
				}
				else {
					ValidNumOfMinesInput = true;
				}
			}
			catch (...) {
				cout << "Not a number! Please try enter the number of mines again: " << endl;
			}
		}








		// variable to determine if you want to play again with the same rules
		bool PlayAgainSameRules = false;

		do {

			// declare and initialise grid with 0's
			vector< vector<int> > Grid(GridSize, vector<int>(GridSize, 0));


			// to declare and initialise visible grid
			vector< vector<char> > VisibleGrid(GridSize, vector<char>(GridSize, NotChosenSymbol));





			// The Start Of The Game Itself
			cout << endl << "Splendid! Enjoy your game!" << endl;
			string ChosenSpot;
			bool GridHasBeenCreated = false;
			bool ShowGrid = true;
			bool ShowDefaultInputQ = true;
			bool ShowHelp = true;
			bool MarkInput;
			int ChosenCoordinates[2] = { 0, 0 };
			int NumberOfMarkedMines = 0;
			int NumberOfWronglyMarkedMines = 0;
			int WhatAfterOptions = 0;
			int GameTimeout = 0;
			while (GameTimeout <= GridSize * GridSize + 100) {

				if (ShowGrid) {
					PrintGrid(VisibleGrid, GridSize);
				}
				ShowGrid = false;

				if (ShowDefaultInputQ) {
					cout << "Choose a grid spot to mark or check: ";
					if (ShowHelp) {
						cout << "\n\nTo check type: 'A5', 'B2, etc." << endl;
						cout << "To mark type: 'A2 M', 'C6 M', etc." << endl;
						cout << "These inputs are NOT case NOR space sensitive. For example 'b3m' also marks B3." << endl;
						cout << "You can type 'QUIT' to quit the game or 'SOL' to see the solution." << endl;
						cout << "You can type 'HELP' to see this message again." << endl;
					}
					else {
						cout << "(Type 'HELP' for instructions.)" << endl;
					}
				}
				ShowDefaultInputQ = true;
				ShowHelp = false;

				getline(cin, ChosenSpot);

				// to check if we got a special instruction
				// -1: QUIT, 1:SOLUTION, 2:HELP
				WhatAfterOptions = CheckForOptions(Grid, GridSize, ChosenSpot, GridHasBeenCreated);
				if (WhatAfterOptions == 1) {
					ShowGrid = true;
					continue;
				}
				else if (WhatAfterOptions == 2) {
					ShowHelp = true;
					cout << endl;
					continue;
				}
				else if (WhatAfterOptions == -1) {
					break;
				}
				else {

				}


				// Checking which spot user picked and if they want to check or mark it
				MarkInput = false; // MarkInput will change to true if user wants to (un)mark
				if (!TryInputToCoordinates(ChosenSpot, ChosenCoordinates, MarkInput)) {
					cout << "Please try to choose a grid spot again to mark or check: (Type 'HELP' for instructions.)" << endl;
					ShowDefaultInputQ = false;
					continue;
				}


				// to check that the intepreted gridspot exists (is withing range)
				if (ChosenCoordinates[0] < 0 || ChosenCoordinates[0] > GridSize - 1 || ChosenCoordinates[1] < 0 || ChosenCoordinates[1] > GridSize - 1) {
					cout << "Chosen coordinates out of range! Please choose a grid spot again to mark or check: (Type 'HELP' for instructions.)" << endl;
					ShowDefaultInputQ = false;
					continue;
				}







				// create the grid after user has chosen the first grid spot to guarantee a good start
				if (!GridHasBeenCreated) {
					CreateGrid(Grid, GridSize, NumberOfMines, ChosenCoordinates);
					GridHasBeenCreated = true;
				}








				// to update visible grid by checking or (un)marking
				if (!MarkInput) {
					CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinates[0], ChosenCoordinates[1]);

					PrintGrid(VisibleGrid, GridSize);

					cout << "You chose to check: " << Coordinate0ToLetters(ChosenCoordinates[0]) << ' ' << ChosenCoordinates[1] + 1 << endl << endl;
					if (VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] == MineSymbol) {
						cout << "\n\n**********You Lost!**********\n\n";
						cout << "\nPress ENTER to see the solution..." << std::flush;
						cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
						Solution(Grid, GridSize);
						cout << "\n\n**********You Lost!**********\n\n";
						break;
					}
				}
				else {
					int GridSpotToDataForMark = Grid[ChosenCoordinates[1]][ChosenCoordinates[0]];
					MarkInputCoordinates(GridSpotToDataForMark, VisibleGrid, ChosenCoordinates, NumberOfMarkedMines, NumberOfWronglyMarkedMines);

					PrintGrid(VisibleGrid, GridSize);

					cout << "You chose to mark: " << Coordinate0ToLetters(ChosenCoordinates[0]) << ' ' << ChosenCoordinates[1] + 1 << endl << endl;
					// Win condition
					if (NumberOfMarkedMines == NumberOfMines && NumberOfWronglyMarkedMines == 0) {
						cout << "\n\n**********You Won!**********\n\n";
						cout << "\nPress ENTER to see the solution..." << std::flush;
						cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
						Solution(Grid, GridSize);
						cout << "\n\n**********You Won!**********\n\n";
						break;
					}
				}
				GameTimeout++;
			}



			cout << "Thank you for playing!" << endl;
			cout << "Would you like to play again? (Y/N)" << endl;
			string PlayAgainStr;
			getline(cin, PlayAgainStr);
			cout << endl;
			if (PlayAgainStr == "Y" || PlayAgainStr == "y" || PlayAgainStr == "1" || PlayAgainStr == "Yes" || PlayAgainStr == "YES" || PlayAgainStr == "yes") {
		
				cout << "Would you like to use the same rule set? (Size of the grid: " << GridSize << 'x' << GridSize << " and the number of mines: " << NumberOfMines << ") (Y/N)" << endl;
				string SameRuleSet;
				getline(cin, SameRuleSet);
				cout << "------------------------------------------" << endl;
				if (SameRuleSet == "Y" || SameRuleSet == "y" || SameRuleSet == "1" || SameRuleSet == "Yes" || SameRuleSet == "YES" || SameRuleSet == "yes") {
					PlayAgainSameRules = true;
				}
				else {
					PlayAgain = true;
				}
			}
		} while (PlayAgainSameRules);

	} while (PlayAgain);

	cout << "Good bye!" << endl;
	cin.get();

	return 0;
}








//--------------------------------------------------------------------------------
//									Definitions:
//--------------------------------------------------------------------------------







// random generator function to use with std::random_shuffle
int MyRandomGen(int i) { return std::rand() % i; }




// to print row of a grid
void PrintRow(vector< vector<char> > &VisibleGrid, int GridSize, int RowNumber) {
	cout << setw(3) << RowNumber+1 << ' ';
	for (int i = 0; i < GridSize; i++) {
		cout << "| " << VisibleGrid[RowNumber][i] << ' ';
	}
	cout << '|' << endl;
}




// to print line between on a grid
void PrintLineRow(int GridSize) {
	cout << "    ";
	for (int i = 0; i < GridSize; i++) {
		cout << "+ - ";
	}
	cout << '+' << endl;
}




// to print letters above a grid
void PrintCharRow(int GridSize) {
	// standard print for grids with size<27
	cout << "    ";
	if (GridSize <= 26) {
		for (int i = 0; i < GridSize; i++) {
			cout << "  " << char(65 + i) << ' ';
		}
	}
	// print for grids size >=27
	if (GridSize > 26 && GridSize <= 675) {
		for (int i = 0; i < 26; i++) {
			cout << "  " << char(65 + i) << ' ';
		}
		int CharNumbOfLastEntry = GridSize / 26 - 1;
		for (int i = 0; i < CharNumbOfLastEntry; i++) {
			for (int j = 0; j < 26; j++)
			cout << "  " << char(65 + i) << char(65 + j);
		}
		for (int i = 0; i < GridSize % 26; i++) {
			cout << "  " << char(65 + CharNumbOfLastEntry) << char(65 + i);
		}
	}

	cout << ' ' << endl;
}




// to print the grid
void PrintGrid(vector< vector<char> > &VisibleGrid, int GridSize) {
	cout << endl;
	PrintCharRow(GridSize);
	for (int i = 0; i < GridSize; i++) {
		PrintLineRow(GridSize);
		PrintRow(VisibleGrid, GridSize, i);
	}
	PrintLineRow(GridSize);
	cout << endl;
}




// to create the data grid with mines
void CreateGrid(vector< vector<int> > &Grid, int GridSize, int NumberOfMines, int(&ChosenCoordinates)[2]) {

	
	// to create a vector with values: 0, 1, 2, ..., GridSize*GridSize - 1
	vector<int> GoodGridSpots;
	for (int i = 0; i < GridSize*GridSize; i++) GoodGridSpots.push_back(i);



	// to shuffle this vector
	std::random_shuffle(GoodGridSpots.begin(), GoodGridSpots.end(), MyRandomGen);



	// to remove bad gridspots (those on and around chosen initial spot)
	GoodGridSpots.erase(std::remove(GoodGridSpots.begin(), GoodGridSpots.end(), 
		(ChosenCoordinates[1] * GridSize + ChosenCoordinates[0])), GoodGridSpots.end());
	if (ChosenCoordinates[0] > 0) {
		GoodGridSpots.erase(std::remove(GoodGridSpots.begin(), GoodGridSpots.end(), 
			(ChosenCoordinates[1] * GridSize + ChosenCoordinates[0] - 1)), GoodGridSpots.end());	//left
	}
	if (ChosenCoordinates[0] < GridSize - 1) {
		GoodGridSpots.erase(std::remove(GoodGridSpots.begin(), GoodGridSpots.end(), 
			(ChosenCoordinates[1] * GridSize + ChosenCoordinates[0] + 1)), GoodGridSpots.end());	// right
	}
	if (ChosenCoordinates[1] > 0) {
		GoodGridSpots.erase(std::remove(GoodGridSpots.begin(), GoodGridSpots.end(), 
			((ChosenCoordinates[1] - 1) * GridSize + ChosenCoordinates[0])), GoodGridSpots.end());	// top
	}
	if (ChosenCoordinates[1] < GridSize - 1) {
		GoodGridSpots.erase(std::remove(GoodGridSpots.begin(), GoodGridSpots.end(), 
			((ChosenCoordinates[1] + 1) * GridSize + ChosenCoordinates[0])), GoodGridSpots.end());	// bottom
	}
	if (ChosenCoordinates[1] > 0 && ChosenCoordinates[0] > 0) {
		GoodGridSpots.erase(std::remove(GoodGridSpots.begin(), GoodGridSpots.end(), 
			((ChosenCoordinates[1] - 1) * GridSize + ChosenCoordinates[0] - 1)), GoodGridSpots.end());	// top left
	}
	if (ChosenCoordinates[1] > 0 && ChosenCoordinates[0] < GridSize - 1) {
		GoodGridSpots.erase(std::remove(GoodGridSpots.begin(), GoodGridSpots.end(), 
			((ChosenCoordinates[1] - 1) * GridSize + ChosenCoordinates[0] + 1)), GoodGridSpots.end());	// top right
	}
	if (ChosenCoordinates[1] < GridSize - 1 && ChosenCoordinates[0] > 0) {
		GoodGridSpots.erase(std::remove(GoodGridSpots.begin(), GoodGridSpots.end(), 
			((ChosenCoordinates[1] + 1) * GridSize + ChosenCoordinates[0] - 1)), GoodGridSpots.end());	// bottom left
	}
	if (ChosenCoordinates[1] < GridSize - 1 && ChosenCoordinates[0] < GridSize - 1) {
		GoodGridSpots.erase(std::remove(GoodGridSpots.begin(), GoodGridSpots.end(), 
			((ChosenCoordinates[1] + 1) * GridSize + ChosenCoordinates[0] + 1)), GoodGridSpots.end());	// bottom right
	}


	// creation of the mines and hints using GoodGridSpots
	int MineX, MineY;
	for (int i = 0; i < NumberOfMines; i++) {

		// to create mines on the grid
		MineX = GoodGridSpots[i] % GridSize;
		MineY = GoodGridSpots[i] / GridSize;

		Grid[MineY][MineX] = 9;

		// to create "hints" around a mine
		if (MineX > 0 && Grid[MineY][MineX - 1] != 9) {
			Grid[MineY][MineX - 1]++;	//left
		}
		if (MineX < GridSize - 1 && Grid[MineY][MineX + 1] != 9) {
			Grid[MineY][MineX + 1]++;	// right
		}
		if (MineY > 0 && Grid[MineY - 1][MineX] != 9) {
			Grid[MineY - 1][MineX]++;	// top
		}
		if (MineY < GridSize - 1 && Grid[MineY + 1][MineX] != 9) {
			Grid[MineY + 1][MineX]++;	// bottom
		}
		if (MineY > 0 && MineX > 0 && Grid[MineY - 1][MineX - 1] != 9) {
			Grid[MineY - 1][MineX - 1]++;	// top left
		}
		if (MineY > 0 && MineX < GridSize - 1 && Grid[MineY - 1][MineX + 1] != 9) {
			Grid[MineY - 1][MineX + 1]++;	// top right
		}
		if (MineY < GridSize - 1 && MineX > 0 && Grid[MineY + 1][MineX - 1] != 9) {
			Grid[MineY + 1][MineX - 1]++;	// bottom left
		}
		if (MineY < GridSize - 1 && MineX < GridSize - 1 && Grid[MineY + 1][MineX + 1] != 9) {
			Grid[MineY + 1][MineX + 1]++;	// bottom right
		}
	}
}




// to transfer input string of coordinates into coordinates for the program (return false if not able to do this)
bool TryInputToCoordinates(string ChosenSpot, int (&ChosenCoordinates)[2], bool &MarkInput) {

	int ChosenSpotLength = ChosenSpot.length();


	// to capitalise input
	transform(ChosenSpot.begin(), ChosenSpot.end(), ChosenSpot.begin(), ::toupper);


	// to remove spaces
	while (ChosenSpot.find(' ') != string::npos) {
		ChosenSpot = ChosenSpot.erase(ChosenSpot.find(' '), 1);
	}


	// to make sure first character is a letter
	int i = 0;
	if (int(ChosenSpot[i]) < 65 || int(ChosenSpot[i]) > 90) {
		cout << "\nFirst character not a letter!" << endl;
		return false;
	}

	// to find when letters turn into numbers
	while (int(ChosenSpot[i]) >= 65 && int(ChosenSpot[i]) <= 90) {
		i++;
		// to make sure input doesn't have only letters (hence options should be checked before this function)
		if (ChosenSpotLength == i) {
			cout << "\nIncorrect input! (input contained only letters, no numbers)" << endl;
			return false;
		}
	}

	int IndexOfFirstNumber = i; // first non-letter (should be a number)
	if (int(ChosenSpot[IndexOfFirstNumber]) < 48 || int(ChosenSpot[IndexOfFirstNumber]) > 57) {
		cout << "\nFirst non-character not a number!" << endl;
		return false;
	}

	
	// Letter(s) to find the first coordinate
	if (IndexOfFirstNumber == 1) {
		ChosenCoordinates[0] = int(ChosenSpot[0]) - 65;
	}
	else if (IndexOfFirstNumber == 2 ) {
		ChosenCoordinates[0] = (int(ChosenSpot[0]) - 64) * 26 + (int(ChosenSpot[1]) - 65);
	}
	else {
		cout << "\nCoordinate corresponding to the given column letters too large!" << endl;
		return false;
	}

	
	// to find when numbers stop
	while (int(ChosenSpot[i]) >= 48 && int(ChosenSpot[i]) <= 57) {
		i++;
		// to make sure we don't try to access non-existent position
		if (ChosenSpotLength == i) break;
	}


	// Numbers to find the second coordinate
	int IndexOfLastNumber = i - 1;
	ChosenCoordinates[1] = stoi(ChosenSpot.substr(IndexOfFirstNumber, IndexOfLastNumber - IndexOfFirstNumber + 1)) - 1;


	// index of M is 'i', if it exists
	if (ChosenSpotLength - 1 >= i) {
		if (ChosenSpot[i] == 'M') {
			MarkInput = true;
		}
	}
	return true;
}




// to transfer intepreted coordinates back to input letters (to let user know, what the program thought they meant)
string Coordinate0ToLetters(int ChosenCoordinate0) {
	if (ChosenCoordinate0 <= 25) {
		string CoordinateToLetter = "A";
		CoordinateToLetter = (ChosenCoordinate0 + 65);
		return CoordinateToLetter;
	}
	else if (ChosenCoordinate0 > 25 && ChosenCoordinate0 < 675) {
		string CoordinateToLetter = "AA";
		CoordinateToLetter[0] = char((ChosenCoordinate0 / 26) + 64 );
		CoordinateToLetter[1] = char((ChosenCoordinate0 % 26) + 65);
		return CoordinateToLetter;
	}
	else {
		cerr << "ERROR: Coordinate to Letters conversion failed!" << endl;
		return "ERROR";
	}
}




// to check user given coordinates, and update result
void CheckInputCoordinates(vector< vector<int> > &Grid, vector< vector<char> > &VisibleGrid, int ChosenCoordinate0, int ChosenCoordinate1) {

	if (VisibleGrid[ChosenCoordinate1][ChosenCoordinate0] == NotChosenSymbol) {

		switch (Grid[ChosenCoordinate1][ChosenCoordinate0]) {
			case 0:
				VisibleGrid[ChosenCoordinate1][ChosenCoordinate0] = NoMinesAroundSymbol;
				CheckAroundCoordinate(Grid, VisibleGrid, ChosenCoordinate0, ChosenCoordinate1);
				break;
			case 1:
				VisibleGrid[ChosenCoordinate1][ChosenCoordinate0] = '1';
				break;
			case 2:
				VisibleGrid[ChosenCoordinate1][ChosenCoordinate0] = '2';
				break;
			case 3:
				VisibleGrid[ChosenCoordinate1][ChosenCoordinate0] = '3';
				break;
			case 4:
				VisibleGrid[ChosenCoordinate1][ChosenCoordinate0] = '4';
				break;
			case 5:
				VisibleGrid[ChosenCoordinate1][ChosenCoordinate0] = '5';
				break;
			case 6:
				VisibleGrid[ChosenCoordinate1][ChosenCoordinate0] = '6';
				break;
			case 7:
				VisibleGrid[ChosenCoordinate1][ChosenCoordinate0] = '7';
				break;
			case 8:
				VisibleGrid[ChosenCoordinate1][ChosenCoordinate0] = '8';
				break;
			case 9:
				VisibleGrid[ChosenCoordinate1][ChosenCoordinate0] = MineSymbol;
				break;
		}
	}
}




// to check around given coordinate
void CheckAroundCoordinate(vector< vector<int> > &Grid, vector< vector<char> > &VisibleGrid, int ChosenCoordinate0, int ChosenCoordinate1) {

	int GridSize = Grid.size();

	if (ChosenCoordinate1 > 0) {
		CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinate0, ChosenCoordinate1 - 1);		// left
	}
	if (ChosenCoordinate1 < GridSize - 1) {
		CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinate0, ChosenCoordinate1 + 1);		// right
	}
	if (ChosenCoordinate0 > 0) {
		CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinate0 - 1, ChosenCoordinate1);		// top
	}
	if (ChosenCoordinate0 < GridSize - 1) {
		CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinate0 + 1, ChosenCoordinate1);		// bottom
	}
	if (ChosenCoordinate0 > 0 && ChosenCoordinate1 > 0) {
		CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinate0 - 1, ChosenCoordinate1 - 1);	// top left
	}
	if (ChosenCoordinate0 > 0 && ChosenCoordinate1 < GridSize - 1) {
		CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinate0 - 1, ChosenCoordinate1 + 1);	// top right
	}
	if (ChosenCoordinate0 < GridSize - 1 && ChosenCoordinate1 > 0) {
		CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinate0 + 1, ChosenCoordinate1 - 1);	// bottom left
	}
	if (ChosenCoordinate0 < GridSize - 1 && ChosenCoordinate1 < GridSize - 1) {
		CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinate0 + 1, ChosenCoordinate1 + 1);	// bottom right
	}
}




// to mark (or unmark) user given coordinates, and update visual result and keeping track of winning progress
void MarkInputCoordinates(int GridSpotToDataForMark, vector< vector<char> > &VisibleGrid, int(&ChosenCoordinates)[2], int &NumberOfMarkedMines, int &NumberOfWronglyMarkedMines) {
	switch (VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]]) {
		case NotChosenSymbol:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = MarkedMineSymbol;
			if (GridSpotToDataForMark == 9) {
				NumberOfMarkedMines++;
			}
			else {
				NumberOfWronglyMarkedMines++;
			}
			break;
		case MarkedMineSymbol:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = NotChosenSymbol;
			if (GridSpotToDataForMark == 9) {
				NumberOfMarkedMines--;
			}
			else {
				NumberOfWronglyMarkedMines--;
			}
			break;
		default:
			break;
	}
}




// to print out the solution
void Solution(vector< vector<int> > Grid, int GridSize) {
	cout << endl;
	cout << "Here is the solution:" << endl << endl;

	cout << setw(5);
	for (int i = 0; i < GridSize; i++) {
		cout << ' ' << char(65 + i);
	}
	cout << setw(10);
	for (int i = 0; i < GridSize; i++) {
		cout << ' ' << char(65 + i);
	}
	cout << endl;

	for (int i = 0; i < GridSize; i++) {
		cout << setw(3) << i + 1 << '|';
		for (int j = 0; j < GridSize; j++) {
			if (Grid[i][j] == 9) {
				cout << ' ' << MineSymbol;
			}
			else if (Grid[i][j] == 0) {
				cout << ' ' << NoMinesAroundSymbol;
			}
			else {
				cout << ' ' <<Grid[i][j];
			}
		}
		cout << setw(8) << i + 1 << '|';
		for (int j = 0; j < GridSize; j++) {
			if (Grid[i][j] == 9) {
				cout << ' ' << '*';
			}
			else {
				cout << ' ' << 0;
			}
		}
		cout << endl;
	}
	cout << "\nPress ENTER to continue..." << std::flush;
	cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
}




// to check for special input options: solution, quit and help
int CheckForOptions(vector< vector<int> > &Grid, int GridSize, string Input, bool GridHasBeenCreated) {
	transform(Input.begin(), Input.end(), Input.begin(), ::toupper);

	// quit program and give solution, if user asks for it
	if (Input == "QUIT" || Input == "Q") {
		cout << "\nI am sorry that you have to leave." << endl;
		if (GridHasBeenCreated) Solution(Grid, GridSize);
		cout << endl;
		return -1;
	}

	if (Input == "HELP" || Input == "H" || Input == "INFO" || Input == "INSTRUCTIONS" || Input == "I") {
		return 2;
	}


	// solution given, if user asks for it
	if (Input == "SOLUTION" || Input == "SOL" || Input == "S") {
		if (GridHasBeenCreated) {
			Solution(Grid, GridSize);
		}
		else {
			cout << "\nThe grid will be created after the first move. Hence solution does not exist, yet." << endl;
			cout << "\nPress ENTER to continue..." << std::flush;
			cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
		}
		cout << endl;
		return 1;
	}

	return 0;
}



