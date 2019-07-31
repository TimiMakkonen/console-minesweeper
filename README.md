# Basic Minesweeper game on console

The purpose of this program is to see/show/test if I can program a minesweeper game on my own.

---

### Note:

I have made sure not to look up codes of any other minesweeper programs prior making this program. Hence this program and the methods used in it to make it work are all mostly 'original'.
Throughout making this program, I have only googled some methods, good practices, algorithms and syntax to make things work the way I want them to.

---

### Screenshots:


#### Random gameplay example:  
![Random gameplay example](/Screenshots/MinesweeperCapture1.JPG "Random example")

#### Start of a game:
![Start of a game](/Screenshots/MinesweeperCapture3.JPG?raw=true "Start of a game")

#### Example of progress after first move:
![Example of progress after first move](/Screenshots/MinesweeperCapture2.JPG?raw=true "Example of first move")

#### New game options:
![New game options](/Screenshots/MinesweeperCapture4.JPG?raw=true "New game")

---

### Version History:

#### Version 5.1:

* Added win condition (all non mines visible/checked).
* Made impossible to mark a cell on the first turn.
* Slightly improved readability of the code.

#### Version 5:

* Updated program from C-style to OOP.
	* New 'Game', 'Grid' and 'Cell' classes.
* Improved readability and efficiency of some parts of code.
	* Changed variable naming convention from pascal to camel and made names easier to understand.
* Changed pseudo-random number generator from 'rand' to 'std::mt19937' (Mersenne Twister 19937) to improve randomness.

#### Version 4:

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

#### Version 3:

* Slightly improved readability of the code.
* Started to deal with proper handling of wrong inputs.
* Added initial win screen and win condition, if you manage to mark all mines.
* Ability to unmark grid spots by marking them again (in case you made a mistake).
* Initial options/special input menu implemented.


#### Version 2:

* Fixed some visual mistakes and made the game look prettier 
* Added automatic mine check for squares around, when choosing a grid location with no surrounding mines.
* Fixes input for 2 digit numbers.
* Initial losing screen.
* More convenient input system.
* \+ Background work to make next additions/fixes easier to implement.

#### Version 1:


Initial version of the game. Mostly in working condition.

---

### Fixes and features left to consider/implement:

* Increasing current maximum gridsize from 99x99? Madness?
* Separating column and row size to allow 'non-square' grids.
* Making sure incorrect inputs are less likely to cause bugs.
* Making the game look prettier.
* Better losing screen.
* Improve Win screen and winning conditions.
* Improve HELP menu.
* Edit input system overall.
* Add an option to add mines by percentage, not by quantity.
* Add an option to choose between 'normal' and 'condensed' grid.
* Change solution screen. (Does not properly work with large grids atm.)

