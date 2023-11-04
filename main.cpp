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
    std::vector<move> legalmoves;
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
            if (legalmoves[0].firstjump) {
                std::cout << "Player 1 must make a jump move" << std::endl;
                for (int i = 0, index = 0; i < legalmoves.size(); i++) {
                    if (legalmoves[i].jumpnumber == 0) {
                        std::cout << std::endl;
                        index++;
                        std::cout << "CHOICE " << index << ": ";
                    }
                    std::cout << legalmoves[i].position << " TO " << legalmoves[i].destination;
                    if (legalmoves[i+1].jumpnumber == 0 || i+1 == legalmoves.size()) {
                    } else {
                        std::cout << " THEN ";
                    }
                }
                std::cout << std::endl;
            } else {
                std::cout << "Player 1 must make regular move" << std::endl;
                for (int i = 0; i < legalmoves.size(); i++) {
                    std::cout << std::endl;
                    std::cout << "CHOICE " << i+1 << ": ";
                    std::cout << legalmoves[i].position << " TO " << legalmoves[i].destination;
                }
                std::cout << std::endl;
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
            int start = 0;
            int counter = 0;
            while (counter < choice) {
                if (legalmoves[start].jumpnumber == 0) {
                    counter++;
                }
                start++;
            }
            chosenmove.push_back(legalmoves[start]);
            start++;
            while (legalmoves[start].jumpnumber != 0) {
                chosenmove.push_back(legalmoves[start]);
                start++;
            }
            checkBoard.applyChoice(chosenmove, checkBoard);
            checkBoard.showBoard(checkBoard);
            checkBoard.playerTurn = false;
        } else { // player 2 turn which is computer
            std::clock_t startTime = std::clock();
            std::cout << "Computer turn: " << std::endl;
            std::vector<move> computermove = checkBoard.alphabeta(checkBoard);
            std::clock_t endTime = std::clock();
            double totalcomputertime = startTime - endTime;
            std::cout << "Clock time: " << totalcomputertime << std::endl;

            checkBoard.applyChoice(computermove, checkBoard);
            checkBoard.showBoard(checkBoard);
            checkBoard.playerTurn = true;
        }
    }
}
