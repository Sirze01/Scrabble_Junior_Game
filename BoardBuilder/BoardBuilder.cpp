#include <iostream>
#include "Board.h"

int main()
{
    std::cout << "Hello world!\n";


    Board newBoard("test.txt");
    newBoard.show();
    std::cout << newBoard.getIndex("Ak").vLine << ' ' << newBoard.getIndex("Ak").hCollumn << std::endl;
    newBoard.fileExport("test_out.txt");
    Board newBoard2(1, 2);
    newBoard2.show();

    std::cout << "Hello world!\n";
    return 0;
}
