#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>




using namespace std;


// to print row of a grid
void PrintRow(vector< vector<char> > &VisibleGrid, int GridSize, int RowNumber) {
	cout << ' ' << RowNumber+1 << ' ';
	for (int i = 0; i < GridSize; i++) {
		cout << "| " << VisibleGrid[RowNumber][i] << ' ';
	}
	cout << '|' << endl;
}

// to print line between on a grid
void PrintLineRow(int GridSize) {
	cout << "   ";
	for (int i = 0; i < GridSize; i++) {
		cout << "+ - ";
	}
	cout << '+' << endl;
}

// to print letters above a grid
void PrintCharRow(int GridSize) {
	cout << "   ";
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
		if (MineX > 0 && Grid[MineY][MineX-1] != 9) Grid[MineY][MineX-1]++;			// left
		if (MineX < GridSize - 1 && Grid[MineY][MineX + 1] != 9) Grid[MineY][MineX+1]++;	// right
		if (MineY > 0 && Grid[MineY - 1][MineX] != 9) Grid[MineY-1][MineX]++;					// top
		if (MineY < GridSize - 1 && Grid[MineY + 1][MineX] != 9) Grid[MineY+1][MineX]++;			// bottom
		if (MineY > 0 && MineX > 0 && Grid[MineY - 1][MineX - 1] != 9) Grid[MineY-1][MineX-1]++;			// top left
		if (MineY > 0 && MineX < GridSize - 1 && Grid[MineY - 1][MineX + 1] != 9) Grid[MineY-1][MineX+1]++;		// top right
		if (MineY < GridSize - 1 && MineX > 0 && Grid[MineY + 1][MineX - 1] != 9) Grid[MineY+1][MineX-1]++;			// bottom left
		if (MineY < GridSize - 1 && MineX < GridSize - 1 && Grid[MineY + 1][MineX + 1] != 9) Grid[MineY+1][MineX+1]++;	// bottom right
	}

}

// to transfer input string of coordinates into coordinates for the program
void InputToCoordinates(string ChosenSpot, int (&ChosenCoordinates)[2]) {
	transform(ChosenSpot.begin(), ChosenSpot.end(), ChosenSpot.begin(), ::toupper);

	ChosenCoordinates[0] = int(ChosenSpot[0])-65;
	ChosenCoordinates[1] = int(ChosenSpot[1])-49;
}


// to check around given coordinate
void CheckArounCoordinate(vector< vector<int> > &Grid, vector< vector<char> > &VisibleGrid, int(&ChosenCoordinates)[2]) {

}




// to check user given coordinates, and update result
void CheckInputCoordinates(vector< vector<int> > &Grid, vector< vector<char> > &VisibleGrid, int (&ChosenCoordinates)[2]) {
	switch (Grid[ChosenCoordinates[1]][ChosenCoordinates[0]]) {
		case 0:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = '0';
				break;
		case 9:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = '*';
				break;
		case 1:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = '1';
				break;
		case 2:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = '2';
				break;
		case 3:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = '3';
				break;
		case 4:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = '4';
				break;
		case 5:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = '5';
				break;
		case 6:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = '6';
				break;
		case 7:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = '7';
				break;
		case 8:
			VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = '8';
				break;
	}
}



// to mark user given coordinates, and update result
void MarkInputCoordinates(vector< vector<char> > &VisibleGrid, int(&ChosenCoordinates)[2]) {
	VisibleGrid[ChosenCoordinates[1]][ChosenCoordinates[0]] = 'B';
}

// Solution
void Solution(vector< vector<int> > Grid, int GridSize) {
	cout << endl;
	cout << "Here is the solution:" << endl << endl;

	for (int i = 0; i < GridSize; i++) {
		for (int j = 0; j < GridSize; j++) {
			if (Grid[i][j] == 9) {
				cout << '*' << ' ';
			}
			else {
				cout << Grid[i][j] << ' ';
			}
		}
		cout << "\t\t";
		for (int j = 0; j < GridSize; j++) {
			if (Grid[i][j] == 9) {
				cout << '*' << ' ';
			}
			else {
				cout << 0 << ' ';
			}
		}
		cout << endl;
	}
}






int main() {

	cout << "Welcome to basic Minesweeper game!" << endl << "First of all, choose desired grid size: (I recommend 10.) " << endl;
	int GridSize;
	cin >> GridSize;
	while (GridSize >= 100) {
		cout << "Too large grid. Choose again: ";
		cin >> GridSize;
	}
	cout << "Perfect! Now choose the number of mines: " << endl;
	int NumberOfMines;
	cin >> NumberOfMines;
	while (NumberOfMines >= GridSize * GridSize) {
		cout << "Too many mines for the grid. Choose again: ";
		cin >> NumberOfMines;
	}
	cout << "Splendid! Enjoy your game!" << endl;

	// declare and initialise grid with 0's
	vector< vector<int> > Grid(GridSize, vector<int>(GridSize, 0));
	CreateGrid(Grid, GridSize, NumberOfMines);

	// Create visible grid
	vector< vector<char> > VisibleGrid(GridSize, vector<char>(GridSize, ' '));
	//CreateVisibleGrid(Grid, VisibleGrid, GridSize);


	// solution for bugging purposes
	Solution(Grid, GridSize);


	string ChosenSpot;
	bool CheckOrMark;
	int ChosenCoordinates[2] = {0, 0};
	int NumberOfMarkedMines = 0;
	while (NumberOfMarkedMines < NumberOfMines) {

		PrintGrid(VisibleGrid, GridSize);

		cout << "Choose a grid spot to mark or check" << endl;
		cin >> ChosenSpot;

		// quit program and give solution, if user asks for it
		if (ChosenSpot == "QUIT" || ChosenSpot == "Quit" || ChosenSpot == "quit" || ChosenSpot == "Q" || ChosenSpot == "q") {
			Solution(Grid, GridSize);
			cout << endl;
			break;
		}


		// solution given, if user asks for it
		if (ChosenSpot == "solution" || ChosenSpot == "Solution" || ChosenSpot == "sol" || ChosenSpot == "Sol" || ChosenSpot == "SOL") {
			Solution(Grid, GridSize);
			cout << endl << "Choose a grid spot to mark or check" << endl;
			cin >> ChosenSpot;
		}
		

		InputToCoordinates(ChosenSpot, ChosenCoordinates);

		// to check for valid input
		while (ChosenCoordinates[0] < 0 || ChosenCoordinates[0] > 24 || ChosenCoordinates[1] < 0 || ChosenCoordinates[1] > 24) {
			cout << "INVALID INPUT! Choose a grid spot again to mark or check" << endl;
			cin >> ChosenSpot;
			InputToCoordinates(ChosenSpot, ChosenCoordinates);
		}


		cout << "Check or mark as bomb? (input '0' to check, '1' to mark)" << endl;
		cin >> CheckOrMark;
		
		// to update visible grid
		if (!CheckOrMark) {
			CheckInputCoordinates(Grid, VisibleGrid, ChosenCoordinates);
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