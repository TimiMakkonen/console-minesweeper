#include <iostream>
#include <random>


#include "Game.h"


std::mt19937 myRandomSeed;

int main() {

	// to set random seed
	std::random_device rd;
	myRandomSeed.seed(rd());

	Minesweeper::Game game;
	game.run();

	std::cout << "You have exited the game." << std::endl;
	
	return 0;
}
