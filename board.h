/*
Pawel Milewski - Fall 2023
Variables and functions for class "board"
*/

#include <vector>
#include <ctime>
#include <unordered_map>
#include "move.h"

#ifndef BOARD_H
#define BOARD_H

class board {
    public:
        board();
        void loadGame(board& inputBoard); //load game from input file
        int boardArray[32] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; //default board setup
        void loadBoard(std::vector<int>& loadBoardArray, board& inputBoard); //load board given board array from inputfile
        void showBoard(board& inputBoard); //show board array in terminal
        bool playerTurn = true; // True for player 1, false for player 2
        int time = 5; // default 5 seconds time
        int maxDepth = 20; //max depth for alphabeta search
        int currentdepth = 2; //starting depth of 2
        std::clock_t timestart; //store start time of computer move to check during iterative deepening
        std::vector<std::vector<move>> legalMoves(board& inputBoard); //obtain legal moves based on board state (which has which player move it is)
        std::vector<move> checkJump(board& inputBoard, int position = -1, int jumpnumber = 0); //check for jumps in the board
        std::vector<move> checkRegular(board& inputBoard); //check for regular moves in board
        board movePiece(move& chosenMove, board& inputBoard); //move a piece on a board (this is used for checking for multiple jump moves)
        void applyChoice(std::vector<move> legalmoves, board& inputBoard); //apply choice of computer or player move to board

        std::vector<move> alphabeta(board& inputBoard, int alpha, int beta, int currentDepth, std::unordered_map<std::string, std::pair<int, std::vector<move>>>& transpositionTable);
        std::pair<int,std::vector<move>> maxValue(board& inputBoard, bool playerTurn, int alpha, int beta, int currentDepth, std::unordered_map<std::string, std::pair<int, std::vector<move>>>& transpositionTable);
        std::pair<int,std::vector<move>> minValue(board& inputBoard, bool playerTurn, int alpha, int beta, int currentDepth, std::unordered_map<std::string, std::pair<int, std::vector<move>>>& transpositionTable);
        int gameisTerminal(board& inputBoard);
        int eval(board& inputBoard);

        std::vector<move> iterativeDeepening(board& inputBoard);
        std::string generateHash(const board& inputBoard); //generate string to store values in transposition table

        bool computergame = false;

};

#endif // BOARD_H