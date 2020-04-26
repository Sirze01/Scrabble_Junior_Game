#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct coord{
    int vLine;
    int hCollumn;
};


class Board {
public:
	//Board();
	Board(int nLines, int nCollumns);
	Board(std::vector<std::vector<char>> letters);
<<<<<<< HEAD
    Board(std::string filename);
    void show() const;
	coord getIndex(std::string position) const;
	void fileExport(std::string filename) const;


=======
	void show() const;
	int getIndex(std::string position) const;
    void fileExport(std::string filename) const;
	void fileImport(std::string filename);
>>>>>>> master
private:

    std::vector<std::vector<char>> _letters;
    unsigned _vDimension;
    unsigned _hDimension;
};

