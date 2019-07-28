#ifndef CELL_H
#define CELL_H

class Cell {
private:

	bool _isVisible = false;
	bool _hasMine = false;
	bool _isMarked = false;
	int _numOfMinesAround = 0;

public:

	bool isVisible() const;
	
	bool hasMine() const;

	bool isMarked() const;

	int numOfMinesAround() const;

	void incrNumOfMinesAround();

	void makeVisible();

	void putMine();

	void markCell();
	
	void unmarkCell();

};

#endif