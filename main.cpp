/*
Pawel Milewski - Fall 2023
Main file to run the program
*/

#include <iostream>
#include <ctime>
#include <vector>
#include <limits>
#include "board.h"
#include "move.h"

int main() {
    board checkBoard = board(); // create board for game
    std::cout << "Load game state? Y/N: ";
    std::string loadYN;
    std::cin >> loadYN;
    if (loadYN == "y" || loadYN == "Y") {
        checkBoard.loadGame(checkBoard); //load game state from file
    } else {
        int inputTime;
        while (true) { //check to make sure input is valid integer
            std::cout << "Input time per move: ";
            if (std::cin >> inputTime) {
                if (inputTime > 0) {
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
        checkBoard.time = inputTime;
    }
    std::cout << "Computer vs. Computer? Y/N: ";
    std::string computervsInput;
    std::cin >> computervsInput;
    std::vector<std::vector<move>> legalmoves; //legal moves is a vector of a vector of moves. std::vector<move> can be either one regular move or one jump move or several jump moves
    if (computervsInput == "y" || computervsInput == "Y") {
        checkBoard.computergame = true;
        while (true) {
            checkBoard.showBoard(checkBoard);
            if (checkBoard.playerTurn == true) { //computer 1 turn which is user
                std::cout << "Computer 1 turn: " << std::endl;
                legalmoves = checkBoard.legalMoves(checkBoard); //legalmoves for computer 1 given board state
                if (legalmoves.empty()) { //if empty, computer 1 lost. either no moves or no pieces
                    std::cout << "Computer 1 lost, no moves" << std::endl;
                    break;
                }
                if (legalmoves[0][0].firstjump) { //forced jumps
                    std::cout << "Computer 1 must make a jump move" << std::endl;
                    for (int i = 0; i < legalmoves.size(); i++) { //iterate through and show legal movesets
                        std::cout << "CHOICE " << i + 1 << ": ";
                        for (int i2 = 0; i2 < legalmoves[i].size(); i2++) {
                            std::cout << legalmoves[i][i2].position << " TO " << legalmoves[i][i2].destination << ", ";
                        }
                        std::cout << std::endl;
                    }
                } else { //regular moves
                    std::cout << "Computer 1 must make regular move" << std::endl;
                    for (int i = 0; i < legalmoves.size(); i++) { //iterate through and show regular moves
                        std::cout << "CHOICE " << i+1 << ": ";
                        std::cout << legalmoves[i][0].position << " TO " << legalmoves[i][0].destination;
                        std::cout << std::endl;
                    }
                }
                std::clock_t startTime = std::clock(); //start clock for computer turn (this is where the iterative deepening alphabetasearch portion actually starts)
                checkBoard.timestart = startTime;      //previous code was just to show user the computers moves
                std::vector<move> computermove = checkBoard.iterativeDeepening(checkBoard); //iterative deepening for alphabetasearch
                std::clock_t endTime = std::clock();
                double totalcomputertime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC; 
                std::cout << "Clock time: " << totalcomputertime << " seconds" << std::endl; //output time passed
                std::cout << "COMPUTER 1 CHOSE: ";
                for (int i = 0; i < computermove.size(); i++) { //output computer 1 move
                    std::cout << computermove[i].position << " TO " << computermove[i].destination << ", "; 
                }
                std::cout << std::endl; 
                checkBoard.applyChoice(computermove, checkBoard); //apply computer 1 move to board
                std::cout << "COMPUTER 1 MOVE APPLIED: SEE BOARD BELOW" << std::endl;
                checkBoard.playerTurn = false; //change turns to computer 2
            } else { //player 2 turn which is computer
                std::cout << "Computer 2 turn: " << std::endl;
                legalmoves = checkBoard.legalMoves(checkBoard); //legalmoves for computer 2 given board state
                if (legalmoves.empty()) { //if empty, computer 2 lost. either no moves or no pieces
                    std::cout << "Computer 2 lost, no moves" << std::endl;
                    break;
                }
                if (legalmoves[0][0].firstjump) { //forced jumps
                    std::cout << "Computer 2 must make a jump move" << std::endl;
                    for (int i = 0; i < legalmoves.size(); i++) { //iterate through and show legal movesets
                        std::cout << "CHOICE " << i + 1 << ": ";
                        for (int i2 = 0; i2 < legalmoves[i].size(); i2++) {
                            std::cout << legalmoves[i][i2].position << " TO " << legalmoves[i][i2].destination << ", ";
                        }
                        std::cout << std::endl;
                    }
                } else { //regular moves
                    std::cout << "Computer 2 must make regular move" << std::endl;
                    for (int i = 0; i < legalmoves.size(); i++) { //iterate through and show regular moves
                        std::cout << "CHOICE " << i+1 << ": ";
                        std::cout << legalmoves[i][0].position << " TO " << legalmoves[i][0].destination;
                        std::cout << std::endl;
                    }
                }
                std::clock_t startTime = std::clock(); //start clock for computer turn (this is where the iterative deepening alphabetasearch portion actually starts)
                checkBoard.timestart = startTime;      //previous code was just to show user the computers moves
                std::vector<move> computermove = checkBoard.iterativeDeepening(checkBoard); //iterative deepening for alphabetasearch
                std::clock_t endTime = std::clock();
                double totalcomputertime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC; 
                std::cout << "Clock time: " << totalcomputertime << " seconds" << std::endl; //output time passed
                std::cout << "COMPUTER 2 CHOSE: ";
                for (int i = 0; i < computermove.size(); i++) { //output computer 2 move
                    std::cout << computermove[i].position << " TO " << computermove[i].destination << ", "; 
                }
                std::cout << std::endl; 
                checkBoard.applyChoice(computermove, checkBoard); //apply computer 2 move to board
                std::cout << "COMPUTER 2 MOVE APPLIED: SEE BOARD BELOW" << std::endl;
                checkBoard.playerTurn = true; //change turns to computer 1
            }
        }
    } else {
        int choice;
        while (true) {
            checkBoard.showBoard(checkBoard); 
            if (checkBoard.playerTurn == true) { //player 1 turn which is user
                std::cout << "Player 1 turn:" << std::endl;
                legalmoves = checkBoard.legalMoves(checkBoard); //get legal moves for player1 given board state
                if (legalmoves.empty()) { //if empty, player 1 lost. either no moves or no pieces
                    std::cout << "Player 1 lost, no moves" << std::endl;
                    break;
                }
                if (legalmoves[0][0].firstjump) { //forced jumps
                    std::cout << "Player 1 must make a jump move" << std::endl;
                    for (int i = 0; i < legalmoves.size(); i++) { //iterate through and show legal movesets
                        std::cout << "CHOICE " << i + 1 << ": ";
                        for (int i2 = 0; i2 < legalmoves[i].size(); i2++) {
                            std::cout << legalmoves[i][i2].position << " TO " << legalmoves[i][i2].destination << ", ";
                        }
                        std::cout << std::endl;
                    }
                } else { //regular moves
                    std::cout << "Player 1 must make regular move" << std::endl;
                    for (int i = 0; i < legalmoves.size(); i++) { //iterate through and show regular moves
                        std::cout << "CHOICE " << i+1 << ": ";
                        std::cout << legalmoves[i][0].position << " TO " << legalmoves[i][0].destination;
                        std::cout << std::endl;
                    }
                }
                while (true) { //check to make sure input is valid integer
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
                checkBoard.applyChoice(legalmoves[choice-1], checkBoard); //apply move to board
                std::cout << "PLAYER MOVE APPLIED: SEE BOARD BELOW" << std::endl;
                checkBoard.playerTurn = false; //change playerturn to player 2
            } else { //player 2 turn which is computer
                std::cout << "Computer turn: " << std::endl;
                legalmoves = checkBoard.legalMoves(checkBoard); //legalmoves for player 2 given board state
                if (legalmoves.empty()) { //if empty, player 2 lost. either no moves or no pieces
                    std::cout << "Computer lost, no moves" << std::endl;
                    break;
                }
                if (legalmoves[0][0].firstjump) { //forced jumps
                    std::cout << "Computer must make a jump move" << std::endl;
                    for (int i = 0; i < legalmoves.size(); i++) { //iterate through and show legal movesets
                        std::cout << "CHOICE " << i + 1 << ": ";
                        for (int i2 = 0; i2 < legalmoves[i].size(); i2++) {
                            std::cout << legalmoves[i][i2].position << " TO " << legalmoves[i][i2].destination << " w" << legalmoves[i][i2].jumpnumber << ", ";
                        }
                        std::cout << std::endl;
                    }
                } else { //regular moves
                    std::cout << "Computer must make regular move" << std::endl;
                    for (int i = 0; i < legalmoves.size(); i++) { //iterate through and show regular moves
                        std::cout << "CHOICE " << i+1 << ": ";
                        std::cout << legalmoves[i][0].position << " TO " << legalmoves[i][0].destination;
                        std::cout << std::endl;
                    }
                }
                std::clock_t startTime = std::clock(); //start clock for computer turn (this is where the iterative deepening alphabetasearch portion actually starts)
                checkBoard.timestart = startTime;      //previous code was just to show user the computers moves
                std::vector<move> computermove = checkBoard.iterativeDeepening(checkBoard); //iterative deepening for alphabetasearch
                std::clock_t endTime = std::clock();
                double totalcomputertime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC; 
                std::cout << "Clock time: " << totalcomputertime << " seconds" << std::endl; //output time passed
                std::cout << "COMPUTER CHOSE: ";
                for (int i = 0; i < computermove.size(); i++) { //output computer move
                    std::cout << computermove[i].position << " TO " << computermove[i].destination << ", "; 
                }
                std::cout << std::endl; 
                checkBoard.applyChoice(computermove, checkBoard); //apply computer move to board
                std::cout << "COMPUTER MOVE APPLIED: SEE BOARD BELOW" << std::endl;
                checkBoard.playerTurn = true; //change turns to user
            }
        }
    }
}
