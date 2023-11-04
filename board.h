#include <vector>
#include <ctime>
#include <unordered_map>
#include "move.h"

#ifndef CHECK_BOARD_H
#define CHECK_BOARD_H

class board {
    public:
        board();
        void loadGame(board& inputBoard);
        int boardArray[32] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        void loadBoard(std::vector<int>& loadBoardArray, board& inputBoard);
        void showBoard(board& inputBoard);
        bool playerTurn = true; // True for player 1, false for player 2
        int time = 5; // default 5 seconds time
        int maxDepth = 20;
        int currentdepth = 2;
        std::clock_t timestart;
        std::vector<std::vector<move>> legalMoves(board& inputBoard);
        std::vector<move> checkJump(board& inputBoard, int position = -1, int jumpnumber = 0);
        std::vector<move> checkRegular(board& inputBoard);
        board movePiece(move& chosenMove, board& inputBoard);
        void applyChoice(std::vector<move> legalmoves, board& inputBoard);

        std::vector<move> alphabeta(board& inputBoard, int alpha, int beta, int currentDepth, std::unordered_map<std::string, std::pair<int, std::vector<move>>>& transpositionTable);
        std::pair<int,std::vector<move>> maxValue(board& inputBoard, bool playerTurn, int alpha, int beta, int currentDepth, std::unordered_map<std::string, std::pair<int, std::vector<move>>>& transpositionTable);
        std::pair<int,std::vector<move>> minValue(board& inputBoard, bool playerTurn, int alpha, int beta, int currentDepth, std::unordered_map<std::string, std::pair<int, std::vector<move>>>& transpositionTable);
        int gameisTerminal(board& inputBoard);
        int eval(board& inputBoard);

        std::vector<move> iterativeDeepening(board& inputBoard);
        std::string generateHash(const board& inputBoard);

};

#endif // CHECK_BOARD_H