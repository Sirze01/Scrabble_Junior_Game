#include <iostream>
#include "Board.h"
#include "BuildBoard.h"

int main()
{
    //std::cout << "Hello world!\n";


    BuildBoard newBoard("test.txt");
    newBoard.show();
    //std::cout << newBoard.getIndex("Ak").vLine << ' ' << newBoard.getIndex("Ak").hCollumn << std::endl;
    newBoard.fileExport("test_out.txt");

    //std::cout << "Hello world!\n";
    return 0;
}
