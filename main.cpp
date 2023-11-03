#include <iostream>
#include <ctime>
#include "board.h"

int main() {
    board initBoard = board();
    std::cout << "Load game state? Y/N: ";
    std::string loadYN;
    std::cin >> loadYN;
    if (loadYN == "y" || loadYN == "Y") {
        initBoard.loadGame();
    }
}
