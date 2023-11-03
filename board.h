#include <vector>

#ifndef CHECK_BOARD_H
#define CHECK_BOARD_H

class board {
    public:
        board();
        board checkBoard;
        void loadGame();
        int boardArray[32] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        void loadBoard(std::vector<int>& loadBoardArray);
};

#endif // CHECK_BOARD_H