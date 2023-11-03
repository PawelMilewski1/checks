#include <vector>
#include "move.h"

#ifndef CHECK_BOARD_H
#define CHECK_BOARD_H

class board {
    public:
        board();
        void loadGame();
        int boardArray[32] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        void loadBoard(std::vector<int>& loadBoardArray);
        void showBoard();
        bool playerTurn = true; // True for player 1, false for player 2
        int time = 5; // default 5 seconds time
        std::vector<move> legalMoves(bool playerTurn);
        std::vector<move> checkJumps(bool playerTurn);
};

#endif // CHECK_BOARD_H