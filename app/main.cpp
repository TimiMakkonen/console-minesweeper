#include <iostream>
#include <random>

#include <minesweeper/random.h>
#include <console-minesweeper/game.h>

int main() {

	// for now, we are happy with 'Random' class provided by minesweeper library to control randomness
	minesweeper::Random random;

	console_minesweeper::Game game(&random);
	game.run();

	std::cout << "You have exited the game." << std::endl;
	
	return 0;
}
