#include <iostream>


#include "Cell.h"



bool Cell::isVisible() const {

	return this->_isVisible;
}



bool Cell::hasMine() const {

	return this->_hasMine;
}



bool Cell::isMarked() const {

	return this->_isMarked;
}



int Cell::numOfMinesAround() const {

	return this->_numOfMinesAround;
}



void Cell::incrNumOfMinesAround() {

	++(this->_numOfMinesAround);
	if (this->_numOfMinesAround < 0 || this->_numOfMinesAround > 9) {
		std::cerr << "Error: Too many mines around cell." << std::endl;
	}
}



void Cell::makeVisible() {

	this->_isVisible = true;
}



void Cell::putMine() {

	this->_hasMine = true;
}



void Cell::markCell() {
	
	if (this->_isMarked) {
		std::cerr << "Error: Trying to double mark cell." << std::endl;
	}
	else {
		this->_isMarked = true;
	}
}



void Cell::unmarkCell() {

	if (!(this->_isMarked)) {
		std::cerr << "Error: Trying to double unmark cell." << std::endl;
	}
	else {
		this->_isMarked = false;
	}
}
