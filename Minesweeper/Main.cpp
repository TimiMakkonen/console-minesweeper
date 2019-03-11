#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;



// symbols for visuals
char MineSymbol = '*';
char MarkedMineSymbol = 'M';
char NoMinesAroundSymbol = char(254);
char NotChosenSymbol = ' ';



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
	int i = 0;
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

	/*
	if (ChosenSpot.length() == 2) {
		ChosenCoordinates[0] = int(ChosenSpot[0])-65;
		ChosenCoordinates[1] = int(ChosenSpot[1])-49;
	}
	else if (ChosenSpot.length() == 3) {
		ChosenCoordinates[0] = int(ChosenSpot[0]) - 65;
		ChosenCoordinates[1] = stoi(ChosenSpot.substr(1))-1;
	}
	else {
		cout << "Wrong input!" << endl;
	}
	*/
}

// prototype
void CheckAroundCoordinate(vector< vector<int> > &Grid, vector< vector<char> > &VisibleGrid, int ChosenCoordinate0, int ChosenCoordinate1);


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




// to mark user given coordinates, and update result
void MarkInputCoordinates(vector< vector<char> > &VisibleGrid, int(&ChosenCoordinates)[2]) {
	VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = MarkedMineSymbol;
}

// to print out the solution
void Solution(vector< vector<int> > Grid, int GridSize) {
	cout << endl;
	cout << "Here is the solution:" << endl << endl;

	cout << setw(5);
	for (int i = 0; i < GridSize; i++) {
		cout << ' ' << char(65 + i);
	}
	cout << "\t" << setw(5);
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
		cout << "\t" << setw(3) << i + 1 << '|';
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
}






int main() {

	cout << "Welcome to basic Minesweeper game!" << endl << "First of all, choose desired grid size: (I recommend 10.) " << endl;
	string GridSizeStr;
	getline(cin,GridSizeStr);
	int GridSize = stoi(GridSizeStr);
	while (GridSize >= 27) {
		cout << "Too large grid. Choose again: ";
		getline(cin, GridSizeStr);
		int GridSize = stoi(GridSizeStr);
	}
	cout << "Perfect! Now choose the number of mines: " << endl;
	string NumberOfMinesStr;
	getline(cin, NumberOfMinesStr);
	int NumberOfMines = stoi(NumberOfMinesStr);
	while (NumberOfMines >= GridSize * GridSize) {
		cout << "Too many mines for the grid. Choose again: ";
		getline(cin, NumberOfMinesStr);
		int NumberOfMines = stoi(NumberOfMinesStr);
	}
	cout << "Splendid! Enjoy your game!" << endl;

	// declare and initialise grid with 0's
	vector< vector<int> > Grid(GridSize, vector<int>(GridSize, 0));
	CreateGrid(Grid, GridSize, NumberOfMines);

	// to declare and initialise visible grid
	vector< vector<char> > VisibleGrid(GridSize, vector<char>(GridSize, NotChosenSymbol));



	// solution for bugging purposes
	Solution(Grid, GridSize);
	


	// The Game Itself
	string ChosenSpot;
	bool MarkInput;
	int ChosenCoordinates[2] = {0, 0};
	int NumberOfMarkedMines = 0;
	while (NumberOfMarkedMines < NumberOfMines) {

		PrintGrid(VisibleGrid, GridSize);

		cout << "Choose a grid spot to mark or check." << endl;
		cout << "To check type: 'A5', 'B2, etc." << endl;
		cout << "To mark type: 'A2 M', 'C6 M', etc." << endl;
		getline(cin, ChosenSpot);

		// quit program and give solution, if user asks for it
		if (ChosenSpot == "QUIT" || ChosenSpot == "Quit" || ChosenSpot == "quit" || ChosenSpot == "Q" || ChosenSpot == "q") {
			cout << "I am sorry that you had to leave." << endl;
			Solution(Grid, GridSize);
			cout << endl;
			break;
		}


		// solution given, if user asks for it
		if (ChosenSpot == "solution" || ChosenSpot == "Solution" || ChosenSpot == "sol" || ChosenSpot == "Sol" || ChosenSpot == "SOL") {
			Solution(Grid, GridSize);
			cout << endl << "Choose a grid spot to mark or check" << endl;
			getline(cin, ChosenSpot);
		}
		
		MarkInput = false;
		InputToCoordinates(ChosenSpot, ChosenCoordinates, MarkInput);


		// to check for valid input
		while (ChosenCoordinates[0] < 0 || ChosenCoordinates[0] > GridSize - 1 || ChosenCoordinates[1] < 0 || ChosenCoordinates[1] > GridSize - 1) {
			cout << "INVALID INPUT! Choose a grid spot again to mark or check" << endl;
			getline(cin, ChosenSpot);
			InputToCoordinates(ChosenSpot, ChosenCoordinates, MarkInput);
		}


		//cout << "Check or mark as bomb? (input '0' to check, '1' to mark)" << endl;
		//cin >> CheckOrMark;
		
		// to update visible grid
		if (!MarkInput) {
			CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinates[0], ChosenCoordinates[1]);
			if (VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] == MineSymbol) {
				cout << endl << "**********You lost!**********" << endl;
				Solution(Grid, GridSize);
				cout << endl << "**********You lost!**********" << endl << endl;
				break;
			}
		}
		else {
			MarkInputCoordinates(VisibleGrid, ChosenCoordinates);
			NumberOfMarkedMines++;
		}


	}


	cout << "Thank you for playing!" << endl;
	system("pause");

	return 0;
}