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








// to print row of a grid
void PrintRow(vector< vector<char> > &VisibleGrid, int GridSize, int RowNumber);

// to print line between on a grid
void PrintLineRow(int GridSize);

// to print letters above a grid
void PrintCharRow(int GridSize);

// to print the grid
void PrintGrid(vector< vector<char> > &VisibleGrid, int GridSize);

// to create the data grid with mines
void CreateGrid(vector< vector<int> > &Grid, int GridSize, int NumberOfMines);

// to transfer input string of coordinates into coordinates for the program
void InputToCoordinates(string ChosenSpot, int(&ChosenCoordinates)[2], bool &MarkInput);

// to check user given coordinates, and update result
void CheckInputCoordinates(vector< vector<int> > &Grid, vector< vector<char> > &VisibleGrid, int ChosenCoordinate0, int ChosenCoordinate1);

// to check around given coordinate
void CheckAroundCoordinate(vector< vector<int> > &Grid, vector< vector<char> > &VisibleGrid, int ChosenCoordinate0, int ChosenCoordinate1);

// to mark (or unmark) user given coordinates, and update visual result and keeping track of winning progress
void MarkInputCoordinates(int GridSpotToDataForMark, vector< vector<char> > &VisibleGrid, int(&ChosenCoordinates)[2], int &NumberOfMarkedMines, int &NumberOfWronglyMarkedMines);

// to print out the solution
void Solution(vector< vector<int> > Grid, int GridSize);

// to check for special input options: solution, quit and help
int CheckForOptions(vector< vector<int> > &Grid, int GridSize, string Input);






//--------------------------------------------------------------------------------
//										Main:
//--------------------------------------------------------------------------------








int main() {

	// Welcome text
	cout << "Welcome to basic Minesweeper game!" << endl;
	cout << "First of all, choose desired grid size: (I recommend 10.) " << endl;


	// Maximum Grid Size
	int MaximumGridSize = 26;


	// Input grid size
	string GridSizeStr;
	bool ValidSizeInput = false;
	int GridSize = 0;
	while (!ValidSizeInput) {
		try {
			getline(cin,GridSizeStr);
			GridSize = stoi(GridSizeStr); // if input (GridSizeStr) is not a number, throws exception
			if (GridSize > MaximumGridSize || GridSize <= 0) {
				cout << "Too small or large number! Choose again: (Choose a whole number between 1 and " << MaximumGridSize << " )" << endl;
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
			if (NumberOfMines >= GridSize * GridSize || NumberOfMines <= 0) {
				cout << "Too many or not enough mines! Choose again: (Choose a whole number between 1 and " << GridSize * GridSize - 1 << " )" << endl;
			}
			else {
				ValidNumOfMinesInput = true;
			}
		}
		catch (...) {
			cout << "Not a number! Please try enter the number of mines again: " << endl;
		}
	}



	// declare and initialise grid with 0's
	vector< vector<int> > Grid(GridSize, vector<int>(GridSize, 0));
	CreateGrid(Grid, GridSize, NumberOfMines);


	// to declare and initialise visible grid
	vector< vector<char> > VisibleGrid(GridSize, vector<char>(GridSize, NotChosenSymbol));




	// solution for bugging purposes
	//Solution(Grid, GridSize);
	


	// The Start Of The Game Itself
	cout << endl << "Splendid! Enjoy your game!" << endl;
	string ChosenSpot;
	bool MarkInput;
	int ChosenCoordinates[2] = {0, 0};
	int NumberOfMarkedMines = 0;
	int NumberOfWronglyMarkedMines = 0;
	int WhatAfterOptions = 0;
	while (true) {

		PrintGrid(VisibleGrid, GridSize);

		cout << "Choose a grid spot to mark or check." << endl;
		cout << "To check type: 'A5', 'B2, etc." << endl;
		cout << "To mark type: 'A2 M', 'C6 M', etc." << endl;
		cout << "These inputs are NOT case NOR space sensitive. For example 'b3m' also marks B3." << endl;
		getline(cin, ChosenSpot);

		// to check if we got a special instruction
		WhatAfterOptions = CheckForOptions(Grid, GridSize, ChosenSpot);
		if (WhatAfterOptions == 1) {
			continue;
		}
		else if (WhatAfterOptions == -1) {
			break;
		}
		else {

		}

	
		// Checking which spot user picked and if they want to check or mark it
		MarkInput = false;
		InputToCoordinates(ChosenSpot, ChosenCoordinates, MarkInput);


		// to check that the intepreted gridspot exists
		while (ChosenCoordinates[0] < 0 || ChosenCoordinates[0] > GridSize - 1 || ChosenCoordinates[1] < 0 || ChosenCoordinates[1] > GridSize - 1) {
			cout << "INVALID INPUT! Choose a grid spot again to mark or check" << endl;
			getline(cin, ChosenSpot);
			InputToCoordinates(ChosenSpot, ChosenCoordinates, MarkInput);
		}


		
		// to update visible grid by checking or (un)marking
		if (!MarkInput) {
			CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinates[0], ChosenCoordinates[1]);
			if (VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] == MineSymbol) {
				cout << "\n\n**********You Lost!**********\n\n";
				Solution(Grid, GridSize);
				cout << "\n\n**********You Lost!**********\n\n";
				break;
			}
		}
		else {
			int GridSpotToDataForMark = Grid[ChosenCoordinates[1]][ChosenCoordinates[0]];
			MarkInputCoordinates(GridSpotToDataForMark, VisibleGrid, ChosenCoordinates, NumberOfMarkedMines, NumberOfWronglyMarkedMines);
			

			// Win condition
			if (NumberOfMarkedMines == NumberOfMines && NumberOfWronglyMarkedMines == 0) {
				cout << "\n\n**********You Won!**********\n\n";				
			}
		}
	}
	


	cout << "Thank you for playing!" << endl;
	cout << "Would you like to play again? (Y/N)" << endl;
	string PlayAgainStr;
	getline(cin, PlayAgainStr);
	if (PlayAgainStr == "Y" || PlayAgainStr == "y" || PlayAgainStr == "1" || PlayAgainStr == "Yes" || PlayAgainStr == "YES" || PlayAgainStr == "yes") {
		cout << "This option is not available yet." << endl;
		//PlayMinesweeperGame();
	}



	cout << "Good bye!" << endl;
	cin.get();

	return 0;
}








//--------------------------------------------------------------------------------
//									Definitions:
//--------------------------------------------------------------------------------








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
	cout << "    ";
	for (int i = 0; i < GridSize; i++) {
		cout << "  " << char(65 + i) << ' ';
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
void CreateGrid(vector< vector<int> > &Grid, int GridSize, int NumberOfMines) {


	srand(time(NULL));
	int MineX, MineY;
	for (int i = 0; i < NumberOfMines; i++) {

		// to create mines on the grid
		do {
			MineX = rand() % GridSize;
			MineY = rand() % GridSize;
		} while (Grid[MineY][MineX] == 9);
		
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




// to transfer input string of coordinates into coordinates for the program
// THIS FUNCTION IN NEED OF MODIFYING! (WORK IN PROGRESS)
void InputToCoordinates(string ChosenSpot, int (&ChosenCoordinates)[2], bool &MarkInput) {
	transform(ChosenSpot.begin(), ChosenSpot.end(), ChosenSpot.begin(), ::toupper);

	// to remove spaces
	while (ChosenSpot.find(' ') != string::npos) {
		ChosenSpot = ChosenSpot.erase(ChosenSpot.find(' '), 1);
	}


	// to find when letters turn into numbers
	unsigned int i = 0;
	if (int(ChosenSpot[i]) <= 65 && int(ChosenSpot[i]) >= 90) {
		cerr << "Input Error: First character not a letter." << endl;
	}
	while (int(ChosenSpot[i]) >= 65 && int(ChosenSpot[i]) <= 90) {
		i++;
	}

	
	int IndexOfFirstNumber = i; // first non-letter (should be a number)
	// First letter(s) to coordinate
	if (IndexOfFirstNumber == 1) {
		ChosenCoordinates[0] = int(ChosenSpot[0]) - 65;
	}
	else if (IndexOfFirstNumber == 2) {
		ChosenCoordinates[0] = int(ChosenSpot[0]) - 65 + int(ChosenSpot[0] - 65); // to be added double character columns
	}
	else {
		cerr << "Input Error in 'Letters' of 'InputToCoordinates'" << endl;
	}

	// to find when numbers stop
	if (int(ChosenSpot[i]) <= 48 || int(ChosenSpot[i]) >= 57) {
		cerr << "Input Error: First non-letter not a number." << endl;
	}
	while (int(ChosenSpot[i]) >= 48 && int(ChosenSpot[i]) <= 57) {
		i++;
	}

	// Numbers to second coordinate
	int IndexOfLastNumber = i - 1;
	if (IndexOfLastNumber - IndexOfFirstNumber == 0) {
		ChosenCoordinates[1] = int(ChosenSpot[IndexOfFirstNumber]) - 49;
	}
	else if (IndexOfLastNumber - IndexOfFirstNumber == 1) {
		ChosenCoordinates[1] = stoi( ChosenSpot.substr(IndexOfFirstNumber, 2) ) - 1;
	}
	else {
		cerr << "Input Error in 'Numbers' of 'InputToCoordinates'" << endl;
	}

	// index of M is 'i', if it exists
	if (ChosenSpot.length() - 1 >= i) {
		if (ChosenSpot[i] == 'M') {
			MarkInput = true;
		}
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
int CheckForOptions(vector< vector<int> > &Grid, int GridSize, string Input) {
	transform(Input.begin(), Input.end(), Input.begin(), ::toupper);

	// quit program and give solution, if user asks for it
	if (Input == "QUIT" || Input == "Q") {
		cout << "\nI am sorry that you have to leave." << endl;
		Solution(Grid, GridSize);
		cout << endl;
		return -1;
	}


	// solution given, if user asks for it
	if (Input == "SOLUTION" || Input == "SOL") {
		Solution(Grid, GridSize);
		cout << endl;
		return 1;
	}

	return 0;
}



