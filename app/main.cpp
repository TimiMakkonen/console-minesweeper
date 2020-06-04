#include <iostream>
#include <random>

#include <vector> //delete after library fix TODO

#include <minesweeper/Random.h>
#include <console-minesweeper/game.h>

int main() {

	// for now, we are happy with 'Random' class provided by minesweeper library to control randomness
	Minesweeper::Random random;

	console_minesweeper::Game game(&random);
	game.run();

	std::cout << "You have exited the game." << std::endl;
	
	return 0;
}
