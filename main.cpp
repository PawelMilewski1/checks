#include <iostream>
#include <ctime>
#include <vector>
#include <limits>
#include "board.h"
#include "move.h"

int main() {
    board checkBoard = board();
    std::cout << "Load game state? Y/N: ";
    std::string loadYN;
    std::cin >> loadYN;
    if (loadYN == "y" || loadYN == "Y") {
        checkBoard.loadGame(checkBoard);
    }
    std::vector<std::vector<move>> legalmoves;
    int choice;
    std::vector<move> chosenmove;
    while (true) {
        checkBoard.showBoard(checkBoard);
        if (checkBoard.playerTurn == true) { //player 1 turn which is user
            std::cout << "Player 1 turn:" << std::endl;
            legalmoves = checkBoard.legalMoves(checkBoard);
            if (legalmoves.empty()) {
                std::cout << "Player 1 lost, no moves" << std::endl;
                break;
            }
            if (legalmoves[0][0].firstjump) {
                std::cout << "Player 1 must make a jump move" << std::endl;
                for (int i = 0; i < legalmoves.size(); i++) {
                    std::cout << "CHOICE " << i + 1 << ": ";
                    for (int i2 = 0; i2 < legalmoves[i].size(); i2++) {
                        std::cout << legalmoves[i][i2].position << " TO " << legalmoves[i][i2].destination << " w" << legalmoves[i][i2].jumpnumber << ", ";
                    }
                    std::cout << std::endl;
                }
            } else {
                std::cout << "Player 1 must make regular move" << std::endl;
                for (int i = 0; i < legalmoves.size(); i++) {
                    std::cout << "CHOICE " << i+1 << ": ";
                    std::cout << legalmoves[i][0].position << " TO " << legalmoves[i][0].destination;
                    std::cout << std::endl;
                }
            }
            while (true) {
                std::cout << "Insert choice: ";
                if (std::cin >> choice) {
                    if (choice > 0 && choice <= legalmoves.size()) {
                        std::cout << std::endl;
                        break;
                    } else {
                        std::cout << std::endl;
                        std::cout << "Invalid choice. ";
                    }
                } else {
                    std::cout << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter an integer." << std::endl;
                }
            }
            checkBoard.applyChoice(legalmoves[choice-1], checkBoard);
            std::cout << "PLAYER MOVE APPLIED: SEE BOARD BELOW" << std::endl;
            checkBoard.playerTurn = false;
        } else { // player 2 turn which is computer
            std::cout << "Computer turn: " << std::endl;
            legalmoves = checkBoard.legalMoves(checkBoard);
            if (legalmoves.empty()) {
                std::cout << "Computer lost, no moves" << std::endl;
                break;
            }
            if (legalmoves[0][0].firstjump) {
                std::cout << "Computer must make a jump move" << std::endl;
                for (int i = 0; i < legalmoves.size(); i++) {
                    std::cout << "CHOICE " << i + 1 << ": ";
                    for (int i2 = 0; i2 < legalmoves[i].size(); i2++) {
                        std::cout << legalmoves[i][i2].position << " TO " << legalmoves[i][i2].destination << " w" << legalmoves[i][i2].jumpnumber << ", ";
                    }
                    std::cout << std::endl;
                }
            } else {
                std::cout << "Computer must make regular move" << std::endl;
                for (int i = 0; i < legalmoves.size(); i++) {
                    std::cout << "CHOICE " << i+1 << ": ";
                    std::cout << legalmoves[i][0].position << " TO " << legalmoves[i][0].destination;
                    std::cout << std::endl;
                }
            }
            std::clock_t startTime = std::clock();
            std::vector<move> computermove = checkBoard.alphabeta(checkBoard);
            std::clock_t endTime = std::clock();
            double totalcomputertime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;
            std::cout << "Clock time: " << totalcomputertime << std::endl;
            std::cout << "COMPUTER CHOSEN: " << computermove[0].position << " TO " << computermove[1].destination << std::endl;
            checkBoard.applyChoice(computermove, checkBoard);
            std::cout << "COMPUTER MOVE APPLIED: SEE BOARD BELOW" << std::endl;
            checkBoard.playerTurn = true;
        }
    }
}
