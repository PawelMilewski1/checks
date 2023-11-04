#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include "board.h"
#include "move.h"

int boardDiagonals[32][4] = {{-1,-1,4,5},{-1,-1,5,6},{-1,-1,6,7},{-1,-1,7,-1},{-1,0,-1,8},{0,1,8,9},{1,2,9,10},{2,3,10,11},{4,5,12,13},{5,6,13,14},{6,7,14,15},{7,-1,15,-1},{-1,8,-1,16},{8,9,16,17},{9,10,17,18},{10,11,18,19},{12,13,20,21},{13,14,21,22},{14,15,22,23},{15,-1,23,-1},{-1,16,-1,24},{16,17,24,25},{17,18,25,26},{18,19,26,27},{20,21,28,29},{21,22,29,30},{22,23,30,31},{23,-1,31,-1},{-1,24,-1,-1},{24,25,-1,-1},{25,26,-1,-1},{26,27,-1,-1}};
int boardtwoDiagonals[32][4] = {{-1,-1,-1,9},{-1,-1,8,10},{-1,-1,9,11},{-1,-1,10,-1},{-1,-1,-1,13},{-1,-1,12,14},{-1,-1,13,15},{-1,-1,14,-1},{-1,1,-1,17},{0,2,16,18},{1,3,17,19},{2,-1,18,-1},{-1,5,-1,21},{4,6,20,22},{5,7,21,23},{6,-1,22,-1},{-1,9,-1,25},{8,10,24,26},{9,11,25,27},{10,-1,26,-1},{-1,13,-1,29},{12,14,28,30},{13,15,29,31},{14,-1,30,-1},{-1,17,-1,-1},{16,18,-1,-1},{17,19,-1,-1},{18,-1,-1,-1},{-1,21,-1,-1},{20,22,-1,-1},{21,23,-1,-1},{22,-1,-1,-1}};

board::board() {

}
std::vector<move> board::alphabeta(board& inputBoard) {
    board alphabetaboard = inputBoard;
    std::pair<int,std::vector<move>> valuemove = maxValue(alphabetaboard, -INT_MAX, INT_MAX, 0);
    return valuemove.second;
}
        
std::pair<int,std::vector<move>> board::maxValue(board& inputBoard, int alpha, int beta, int currentDepth) {
    int gameisTerminal = inputBoard.gameisTerminal(inputBoard);
    std::vector<move> move;
    if (gameisTerminal != 0) { // THEN GAME IS TERMINAL
        return std::make_pair(gameisTerminal, move); // return utility and null
    }
    if (currentDepth == inputBoard.maxDepth) {
        return std::make_pair(inputBoard.eval(inputBoard), move);
    }
    int v = -INT_MAX;
    std::vector<move> legalmoves = inputBoard.legalMoves(inputBoard);
    std::vector<move> moveset;
    for (int i = 0; i < legalmoves.size(); i++) {
        board searchboard = inputBoard;
        
        while (legalmoves[i].jumpnumber != 0) {
            moveset.pushback(legalmoves[i]);
            i++;
        }

        searchboard.applyChoice(moveset, searchboard);
        std::pair<int,std::vector<move>> minvaluemove = minValue(searchboard, alpha, beta, currentDepth+1);
        if (minvaluemove.first > v) {
            v = minvaluemove.first;
            move = moveset;
            alpha = std::max(alpha,v);
        }
        if (v >= beta) {
            return std::make_pair(v, move);
        }
    }
    return std::make_pair(v,move);
}
        
std::pair<int,std::vector<move>> board::minValue(board& inputBoard, int alpha, int beta, int currentDepth) {
    int gameisTerminal = inputBoard.gameisTerminal(inputBoard);
    std::vector<move> move;
    if (gameisTerminal != 0) { // THEN GAME IS TERMINAL
        return std::make_pair(gameisTerminal, move); // return utility and null
    }
    if (currentDepth == inputBoard.maxDepth) {
        return std::make_pair(inputBoard.eval(inputBoard), move);
    }
    int v = INT_MAX;
    std::vector<move> legalmoves = inputBoard.legalMoves(inputBoard);
    std::vector<move> moveset;
    for (int i = 0; i < legalmoves.size(); i++) {
        board searchboard = inputBoard;
         
        while (legalmoves[i].jumpnumber != 0) {
            moveset.pushback(legalmoves[i]);
            i++;
        }

        searchboard.applyChoice(moveset, searchboard);
        std::pair<int,std::vector<move>> maxvaluemove = maxValue(searchboard, alpha, beta, currentDepth+1);
        if (maxvaluemove.first < v) {
            v = maxvaluemove.first;
            move = moveset;
            beta = std::min(beta,v);
        }
        if (v <= alpha) {
            return std::make_pair(v, move);
        }
    }
    return std::make_pair(v,move);
}

int board::eval(board& inputBoard) {
    int manCount = 0;
    int kingCount = 0;
    int eval = 0;
    for (int i = 0; i < 32; i++) {
        if (inputBoard.boardArray[i] == 1) { 
            manCount--;
        }
        if (inputBoard.boardArray[i] == 2) { 
            manCount++;
        }
        if (inputBoard.boardArray[i] == 3) { 
            kingCount--;
        }
        if (inputBoard.boardArray[i] == 4) { 
            kingCount++;
        }
    }
    eval = manCount + kingCount * 2;
    return eval;
}

int board::gameisTerminal(board& inputBoard) {
    if (inputBoard.playerTurn) {
        std::vector<move> legalmovesPlayer1 = legalMoves(inputBoard);
        if (legalmovesPlayer1.empty()) {
            return INT_MAX; // PLAYER 2 WINS
        }
    } else {
        std::vector<move> legalmovesPlayer2 = legalMoves(inputBoard);
        if (legalmovesPlayer2.empty()) {
            return -INT_MAX; // PLAYER 1 WINS
        }
    }
    return 0; // GAME IS NOT TERMINAL
}

std::vector<move> board::legalMoves(board& inputBoard) {
    std::vector<move> legalmoves;
    legalmoves = checkJump(inputBoard, -1, 0);
    if (legalmoves.empty()) { // then there are no jumps available
        legalmoves = checkRegular(inputBoard);
    }
    return legalmoves;
}

void board::applyChoice(std::vector<move> legalmoves, board& inputBoard) {
    if (!legalmoves[0].jump) {
        movePiece(legalmoves[0], inputBoard);
    } else {
        for (int i = 0; i < legalmoves.size(); i++) {
            movePiece(legalmoves[i], inputBoard);
        }
    }
}

board board::movePiece(move& chosenMove, board& inputBoard) {
    int jumpPiece;
    jumpPiece = inputBoard.boardArray[chosenMove.position];
    if (chosenMove.promote) {
        if (jumpPiece == 1) {
            jumpPiece = 3;
        }
        if (jumpPiece == 2) {
            jumpPiece = 4;
        }
    }
    if (chosenMove.jump) {
        inputBoard.boardArray[chosenMove.position] = 0;
        inputBoard.boardArray[chosenMove.jumpedover] = 0;
        inputBoard.boardArray[chosenMove.destination] = jumpPiece;
    } else {
        inputBoard.boardArray[chosenMove.position] = 0;
        inputBoard.boardArray[chosenMove.destination] = jumpPiece;
    }
    return inputBoard;
}

std::vector<move> board::checkRegular(board& inputBoard) {
    int player = 1;
    int enemy = 2;
    bool promote = false;
    if (!inputBoard.playerTurn) {
        enemy = 1;
        player = 2;
    }
    std::vector<move> regularmoves;
    move regularmove;
    for (int i = 0; i < 32; i++) {
        if (inputBoard.boardArray[i] == player + 2 || (player == 1 && inputBoard.boardArray[i] == player)) {
            for (int i2 = 0; i2 < 2; i2++) {
                if (inputBoard.boardArray[boardDiagonals[i][i2]] == 0 && boardDiagonals[i][i2] != -1) {
                    regularmove.position = i;
                    regularmove.destination = boardDiagonals[i][i2];
                    if (regularmove.destination < 4 || regularmove.destination > 27) {
                        promote = true;
                    }
                    regularmove.promote = promote;
                    regularmoves.emplace_back(regularmove);
                }
            }
        }
        if (inputBoard.boardArray[i] == player + 2 || (player == 2 && inputBoard.boardArray[i] == player)) {
            for (int i2 = 2; i2 < 4; i2++) {
                if (inputBoard.boardArray[boardDiagonals[i][i2]] == 0 && boardDiagonals[i][i2] != -1) {
                    regularmove.position = i;
                    regularmove.destination = boardDiagonals[i][i2];
                    if (regularmove.destination < 4 || regularmove.destination > 27) {
                        promote = true;
                    }
                    regularmove.promote = promote;
                    regularmoves.emplace_back(regularmove);
                }
            }
        }
    }
    return regularmoves;
}

std::vector<move> board::checkJump(board& inputBoard, int position, int jumpnumber) {
    board jumpboard = inputBoard;
    int player = 1;
    int enemy = 2;
    bool promote = false;
    if (!inputBoard.playerTurn) {
        enemy = 1;
        player = 2;
    }
    std::vector<move> jumpmoves;
    std::vector<move> returnedjumpmoves;
    move currentmove;
    if (position > 0) {
        if (jumpboard.boardArray[position] == player + 2 || (player == 1 && jumpboard.boardArray[position] == player)) {
            for (int i2 = 0; i2 < 2; i2++) {
                if (boardtwoDiagonals[position][i2] > -1) {
                    if ((jumpboard.boardArray[boardDiagonals[position][i2]] == enemy || jumpboard.boardArray[boardDiagonals[position][i2]] == enemy + 2) && jumpboard.boardArray[boardtwoDiagonals[position][i2]] == 0) {
                        if (boardtwoDiagonals[position][i2] < 4 || boardtwoDiagonals[position][i2] > 27) {
                            promote = true;
                        }
                        currentmove.position = position;
                        currentmove.destination = boardtwoDiagonals[position][i2];
                        currentmove.jumpedover = boardDiagonals[position][i2];
                        currentmove.promote = promote;
                        currentmove.player = inputBoard.playerTurn;
                        currentmove.jump = true; 
                        currentmove.jumpnumber = jumpnumber;

                        jumpboard = jumpboard.movePiece(currentmove, jumpboard);
                        jumpmoves.emplace_back(currentmove);
                        returnedjumpmoves = jumpboard.checkJump(jumpboard, currentmove.destination, jumpnumber+1);
                        if (!returnedjumpmoves.empty()) {
                            for (int i = 0; i < returnedjumpmoves.size(); ++i) {
                                jumpmoves.push_back(returnedjumpmoves[i]);
                            }
                        }
                    }
                }
            }
        }
        if (jumpboard.boardArray[position] == player + 2 || (player == 2 && jumpboard.boardArray[position] == player)) {
            for (int i2 = 2; i2 < 4; i2++) {
                if (boardtwoDiagonals[position][i2] > -1) {
                    if ((jumpboard.boardArray[boardDiagonals[position][i2]] == enemy || jumpboard.boardArray[boardDiagonals[position][i2]] == enemy + 2) && jumpboard.boardArray[boardtwoDiagonals[position][i2]] == 0) {
                        if (boardtwoDiagonals[position][i2] < 4 || boardtwoDiagonals[position][i2] > 27) {
                            promote = true;
                        }
                        currentmove.position = position;
                        currentmove.destination = boardtwoDiagonals[position][i2];
                        currentmove.jumpedover = boardDiagonals[position][i2];
                        currentmove.promote = promote;
                        currentmove.player = inputBoard.playerTurn;
                        currentmove.jump = true; 
                        currentmove.jumpnumber = jumpnumber;

                        jumpboard = jumpboard.movePiece(currentmove, jumpboard);
                        jumpmoves.emplace_back(currentmove);
                        returnedjumpmoves = jumpboard.checkJump(jumpboard, currentmove.destination, jumpnumber+1);
                        if (!returnedjumpmoves.empty()) {
                            for (int i = 0; i < returnedjumpmoves.size(); ++i) {
                                jumpmoves.push_back(returnedjumpmoves[i]);
                            }
                        }
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < 32; i++) {
            if (jumpboard.boardArray[i] == player + 2 || (player == 1 && jumpboard.boardArray[i] == player)) {
                for (int i2 = 0; i2 < 2; i2++) {
                    if (boardtwoDiagonals[i][i2] > -1) {
                        if ((jumpboard.boardArray[boardDiagonals[i][i2]] == enemy || jumpboard.boardArray[boardDiagonals[i][i2]] == enemy + 2) && jumpboard.boardArray[boardtwoDiagonals[i][i2]] == 0) {
                            if (boardtwoDiagonals[i][i2] < 4 || boardtwoDiagonals[i][i2] > 27) {
                                promote = true;
                            }
                            currentmove.position = i;
                            currentmove.destination = boardtwoDiagonals[i][i2];
                            currentmove.jumpedover = boardDiagonals[i][i2];
                            currentmove.promote = promote;
                            currentmove.player = inputBoard.playerTurn;
                            currentmove.jump = true;
                            currentmove.firstjump = true;
                            currentmove.jumpnumber = jumpnumber;

                            jumpboard = jumpboard.movePiece(currentmove, jumpboard);
                            jumpmoves.emplace_back(currentmove);
                            returnedjumpmoves = jumpboard.checkJump(jumpboard, currentmove.destination, jumpnumber+1);
                            if (!returnedjumpmoves.empty()) {
                                for (int i = 0; i < returnedjumpmoves.size(); ++i) {
                                    jumpmoves.push_back(returnedjumpmoves[i]);
                                }
                            }
                            jumpboard = inputBoard;
                        }
                    }
                }
            }
            if (jumpboard.boardArray[i] == player + 2 || (player == 2 && jumpboard.boardArray[i] == player)) {
                for (int i2 = 2; i2 < 4; i2++) {
                    if (boardtwoDiagonals[i][i2] > -1) {
                        if ((jumpboard.boardArray[boardDiagonals[i][i2]] == enemy || jumpboard.boardArray[boardDiagonals[i][i2]] == enemy + 2) && jumpboard.boardArray[boardtwoDiagonals[i][i2]] == 0) {
                            if (boardtwoDiagonals[i][i2] < 4 || boardtwoDiagonals[i][i2] > 27) {
                                promote = true;
                            }
                            currentmove.position = i;
                            currentmove.destination = boardtwoDiagonals[i][i2];
                            currentmove.jumpedover = boardDiagonals[i][i2];
                            currentmove.promote = promote;
                            currentmove.player = inputBoard.playerTurn;
                            currentmove.jump = true;
                            currentmove.firstjump = true;
                            currentmove.jumpnumber = jumpnumber;

                            jumpboard = jumpboard.movePiece(currentmove, jumpboard);
                            jumpmoves.emplace_back(currentmove);
                            returnedjumpmoves = jumpboard.checkJump(jumpboard, currentmove.destination, jumpnumber+1);
                            if (!returnedjumpmoves.empty()) {
                                for (int i = 0; i < returnedjumpmoves.size(); ++i) {
                                    jumpmoves.push_back(returnedjumpmoves[i]);
                                }
                            }
                            jumpboard = inputBoard;
                        }
                    }
                }
            }
        }//  move(int positionInput, int destinationInput,int removedPosInput, bool reachedEndInput, bool playerInput) {
    }
    return jumpmoves;
}

void board::loadBoard(std::vector<int>& loadBoardArray, board& inputBoard) {
    for (int i = 0; i < 32; i++) {
        inputBoard.boardArray[i] = loadBoardArray[i];
    }
}

void board::showBoard(board& inputBoard) {
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

void board::loadGame(board& inputBoard) {
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
        inputBoard.loadBoard(loadBoardArray, inputBoard);
        std::getline(inputFile,line);
        if (line[0] == '2') {
            inputBoard.playerTurn = false; // it's player two turn
        }

        std::getline(inputFile,line);
        inputBoard.time = line[0] - '0';

        inputFile.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
}