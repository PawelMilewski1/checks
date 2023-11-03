#include <iostream>
#include <fstream>
#include <vector>
#include "board.h"

board::board() {

}

std::vector<move> legalMoves(bool playerTurn) {
    if (checkJumps(this->playerTurn).empty()) {

    }
}

std::vector<move> checkJump(bool playerTurn) {
    int enemy = 2;
    bool promote = false;
    if (!playerTurn) {
        enemy = 1;
    }
    std::vector<move> jumpMoves;
    for (int i = 0; i < 32; i++) {
        for (int i2 = 0; i2 < 2; i2++) { 
            if (boardDiagonals[i][i2] == enemy && boardtwoDiagonals[i][i2] == 0) {
            if (i2 < 4 || i2 > 27) {
                promote = true;
            }
            moves.emplace_back(this->boardArray[i], boardtwoDiagonals[i][i2], boardDiagonals[i][i2], promote, playerTurn);
           } 
        }
        if (this->boardArray[i] > 2) {
            for (int i2 = 2; i2 < 4; i2++) {
                if (boardDiagonals[i][i2] == enemy && boardtwoDiagonals[i][i2] == 0) {
                if (i2 < 4 || i2 > 27) {
                    promote = true;
                }
                moves.emplace_back(this->boardArray[i], boardtwoDiagonals[i][i2], boardDiagonals[i][i2], promote, playerTurn);
                } 
            }
        }//  move(int positionInput, int destinationInput,int removedPosInput, bool reachedEndInput, bool playerInput) {
    }
}

void board::loadBoard(std::vector<int>& loadBoardArray) {
    for (int i = 0; i < 32; i++) {
        this->boardArray[i] = loadBoardArray[i];
    }
}

void board::showBoard() {
    int row = 0;
    for (int i = 0; i < sizeof(boardArray) / sizeof(boardArray[0]); i++) {
        if (row%2 == 0) {
            std::cout << "  " << boardArray[i];
        } else {
            std::cout << boardArray[i] << "  ";
        }
        if ((i+1)%4 == 0) {
            std::cout << std::endl;
            row++;
        }
    }
}

void board::loadGame() {
    std::cout << "Filename: ";
    std::string loadFilename;
    std::cin >> loadFilename;
    std::ifstream inputFile(loadFilename);

    if (inputFile.is_open()) {
        std::string line;
        std::vector<int> loadBoardArray;
        int lineNumber = 0;
        while (lineNumber < 8 && std::getline(inputFile, line)) {
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == '0' || line[i] == '1' || line[i] == '2' || line[i] == '3' || line[i] == '4') {
                    loadBoardArray.push_back(line[i] - '0');
                }
            }
            lineNumber++;
        }        
        this->loadBoard(loadBoardArray);
        std::getline(inputFile,line);
        if (line[0] == '2') {
            this->playerTurn = false; // it's player two turn
        }

        std::getline(inputFile,line);
        this->playerTurn = std::stoi(line[0]);

        inputFile.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
}