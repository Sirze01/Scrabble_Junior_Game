#include <iostream>
#include "Board.h"

int main()
{
    std::cout << "Hello world!\n";


    Board newBoard("test.txt");
    newBoard.show();
    std::cout << newBoard.getIndex("Ak").vLine << ' ' << newBoard.getIndex("Ak").hCollumn << std::endl;
    newBoard.fileExport("test_out.txt");

    std::cout << "Hello world!\n";
    return 0;
}
