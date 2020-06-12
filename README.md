# Basic Minesweeper game on console

Console version of Minesweeper game written in C++ utilising [Minesweeper game library](https://github.com/TimiMakkonen/minesweeper) version 8.1.0.

---

## How to use

If you want to clone this git repository, use

```console
git clone --recurse-submodules https://github.com/TimiMakkonen/console-minesweeper.git
```

or something similar to make sure that 'minesweeper' submodule gets cloned properly.

Then you can install this game on your machine using CMake in your preferred way.

---

## Screenshots

### Random gameplay example

![Random gameplay example](/screenshots/MinesweeperCapture1.JPG "Random example")

### Start of a game

![Start of a game](/screenshots/MinesweeperCapture3.JPG "Start of a game")

### Example of progress after first move

![Example of progress after first move](/screenshots/MinesweeperCapture2.JPG "Example of first move")

### New game options

![New game options](/screenshots/MinesweeperCapture4.JPG "New game")

---

## Version History

### Version 7.1.1

* Updated application to use [Minesweeper game library](https://github.com/TimiMakkonen/minesweeper) version 8.0.0.
* Slightly edited 'README.MD'.

### Version 7.1.0

* Updated application to use [Minesweeper game library](https://github.com/TimiMakkonen/minesweeper) version 8.0.0 by changing appropriate header, variable and class names.

### Version 7.0.0

* Separated original Minesweeper game (on console) into:
  * ['minesweeper'](https://github.com/TimiMakkonen/minesweeper) (game library) and
  * 'console-minesweeper' (console version of the game which uses the aforementioned library).
* Turned VS solution into CMake app which uses minesweeper library.
  * Added '(google)testing'. (Not implemented yet.)
* Changed file naming convention from 'PascalCase' to 'snake_case'.
* Changed namespace naming convention from 'PascalCase' to 'lowercase'.
* Uses [Minesweeper game library](https://github.com/TimiMakkonen/minesweeper) version 7.0.0.

### Version 6.1.1

* Added validity check for the number of mines in the constructor of 'Grid'.
* Created static methods in 'Grid' class to tell the maximum and minimum number of mines allowed in a grid of given size.

### Version 6.1.0

* Separated 'gridSize' into 'gridHeight' and 'gridWidth' to allow 'non-square' grids both in 'Grid' and 'Game'.

### Version 6.0

* Moved print functionality from 'Grid' to 'Game'.
* Changed cerr's into asserts and throws.
* Moved and changed some of the game logic.

### Version 5.2

* Slightly improved readability of the code and fixed minor things.
* Changed minimum size of grid into 4x4 to stop softlock when choosing number of mines for smaller grids.

### Version 5.1

* Added win condition (all non mines visible/checked).
* Made impossible to mark a cell on the first turn.
* Slightly improved readability of the code.

### Version 5.0

* Updated program from C-style to OOP.
  * New 'Game', 'Grid' and 'Cell' classes.
* Improved readability and efficiency of some parts of code.
  * Changed variable naming convention from pascal to camel and made names easier to understand.
* Changed pseudo-random number generator from 'rand' to 'std::mt19937' (Mersenne Twister 19937) to improve randomness.

### Version 4.0

* Continued with improvements on the input system.
* Increased maximum size of the grid up to 99x99. (I do not recommend grids larger than 50x50.)
* Program now shows your last grid spot input.
* Made function 'TryInputToCoordinates' mostly future proof.
  * can handle grid sizes up to 675 (overkill tbh)
  * able to give error messages and decide when new input is needed.
* Improved games structure for all the different possibilities.
* Added and made HELP menu optional. (show up in the beginning and later upon request)
* Improved instructions and other text output.
* Added ability to play again.
* Made sure that you cannot lose on the first turn.
  * Creation of the grid has been moved to take into account players first choice, making sure player will make some progress.

### Version 3.0

* Slightly improved readability of the code.
* Started to deal with proper handling of wrong inputs.
* Added initial win screen and win condition, if you manage to mark all mines.
* Ability to unmark grid spots by marking them again (in case you made a mistake).
* Initial options/special input menu implemented.

### Version 2.0

* Fixed some visual mistakes and made the game look prettier.
* Added automatic mine check for squares around, when choosing a grid location with no surrounding mines.
* Fixes input for 2 digit numbers.
* Initial losing screen.
* More convenient input system.
* \+ Background work to make next additions/fixes easier to implement.

### Version 1.0

Initial version of the game. Mostly in working condition.

---

## Fixes and features left to consider/implement

* Increasing current maximum gridsize from 99x99? Madness?
* Making sure incorrect inputs are less likely to cause bugs.
* Making the game look prettier.
* Better losing screen.
* Improve Win screen and winning conditions.
* Improve HELP menu.
* Edit input system overall.
* Add an option to add mines by percentage, not by quantity.
* Add an option to choose between 'normal' and 'condensed' grid.
* Change solution screen. (Does not properly work with large grids atm.)
* Add tests.
* Add proper documentation.
