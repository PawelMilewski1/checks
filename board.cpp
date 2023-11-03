#include <iostream>
#include <fstream>
#include <vector>
#include "board.h"

board::board() {

}

void board::loadBoard(std::vector<int>& loadBoardArray) {
    for (int i = 0; i < 32; i++) {
        boardArray[i] = loadBoardArray[i];
    }
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
        checkBoard.loadBoard(loadBoardArray);
        std::getline(inputFile,line);
        if (line[0] == '1') {
            // Decide whose turn it is
        }

        std::getline(inputFile,line);
        // time limit is std::stoi(line[0])

        inputFile.close();
    } else {
        std::cerr << "Unable to open the file." << std::endl;
    }
}