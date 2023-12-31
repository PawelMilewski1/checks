/*
Pawel Milewski - Fall 2023
Definitions for functions for class "board"
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <random>
#include <ctime>
#include <unordered_map>
#include "board.h"
#include "move.h"
//diagonals and double diagonals used for checking moves
int boardDiagonals[32][4] = {{-1,-1,4,5},{-1,-1,5,6},{-1,-1,6,7},{-1,-1,7,-1},{-1,0,-1,8},{0,1,8,9},{1,2,9,10},{2,3,10,11},{4,5,12,13},{5,6,13,14},{6,7,14,15},{7,-1,15,-1},{-1,8,-1,16},{8,9,16,17},{9,10,17,18},{10,11,18,19},{12,13,20,21},{13,14,21,22},{14,15,22,23},{15,-1,23,-1},{-1,16,-1,24},{16,17,24,25},{17,18,25,26},{18,19,26,27},{20,21,28,29},{21,22,29,30},{22,23,30,31},{23,-1,31,-1},{-1,24,-1,-1},{24,25,-1,-1},{25,26,-1,-1},{26,27,-1,-1}};
int boardtwoDiagonals[32][4] = {{-1,-1,-1,9},{-1,-1,8,10},{-1,-1,9,11},{-1,-1,10,-1},{-1,-1,-1,13},{-1,-1,12,14},{-1,-1,13,15},{-1,-1,14,-1},{-1,1,-1,17},{0,2,16,18},{1,3,17,19},{2,-1,18,-1},{-1,5,-1,21},{4,6,20,22},{5,7,21,23},{6,-1,22,-1},{-1,9,-1,25},{8,10,24,26},{9,11,25,27},{10,-1,26,-1},{-1,13,-1,29},{12,14,28,30},{13,15,29,31},{14,-1,30,-1},{-1,17,-1,-1},{16,18,-1,-1},{17,19,-1,-1},{18,-1,-1,-1},{-1,21,-1,-1},{20,22,-1,-1},{21,23,-1,-1},{22,-1,-1,-1}};

board::board() {

}
std::vector<move> board::iterativeDeepening(board& inputBoard) {
    std::vector<move> bestMove;
    std::unordered_map<std::string, std::pair<int, std::vector<move>>> transpositionTable;
    // depth iterates by +2 so that maxvalue is called last instead of minvalue (otherwise best move for player 1 was returned) so then time/4 is used instead of time/2
    for (int depth = 2; depth <= inputBoard.maxDepth && (static_cast<double>(std::clock() - inputBoard.timestart) / CLOCKS_PER_SEC < inputBoard.time / 4); depth+=2) {
        inputBoard.currentdepth = depth;
        //std::cout << "DEPTH REACHED: " << inputBoard.currentdepth << std::endl;
        //std::cout << static_cast<double>(std::clock() - inputBoard.timestart) / CLOCKS_PER_SEC << std::endl;
        bestMove = alphabeta(inputBoard, -INT_MAX, INT_MAX, 0, transpositionTable);
    }
    std::cout << "DEPTH REACHED: " << inputBoard.currentdepth << std::endl;
    return bestMove;
}

std::string board::generateHash(const board& inputBoard) { // create unique string key to input into data into transpositiontable
    std::string hash;

    for (int i = 0; i < 32; i++) {
        hash += std::to_string(inputBoard.boardArray[i]);
        if (i < 32 - 1) {
            hash += '|';
        }
    }

    hash += '|' + (inputBoard.playerTurn ? "1" : "0");
    hash += '|' + std::to_string(inputBoard.currentdepth);

    return hash;
}

std::vector<move> board::alphabeta(board& inputBoard, int alpha, int beta, int currentDepth, std::unordered_map<std::string, std::pair<int, std::vector<move>>>& transpositionTable) {

    if (currentDepth == 0) {
        std::string boardHash = generateHash(inputBoard);
        if (transpositionTable.find(boardHash) != transpositionTable.end()) {
            return transpositionTable[boardHash].second;
        }
    }

    board alphabetaboard = inputBoard;
    std::vector<std::vector<move>> checkforonemove = inputBoard.legalMoves(inputBoard);
    if (checkforonemove.size() == 1) { // if have one move, choose it
        return checkforonemove[0];
    }

    std::pair<int,std::vector<move>> valuemove = maxValue(alphabetaboard, alphabetaboard.playerTurn, -INT_MAX, INT_MAX, currentDepth, transpositionTable);

    return valuemove.second;
}
        
std::pair<int,std::vector<move>> board::maxValue(board& inputBoard, bool playerTurn, int alpha, int beta, int currentDepth, std::unordered_map<std::string, std::pair<int, std::vector<move>>>& transpositionTable) {
    if (!inputBoard.computergame) {
        inputBoard.playerTurn = false; //max value is always going to be player 2 (computer)
    }

    std::string boardHash = generateHash(inputBoard);

    if (transpositionTable.find(boardHash) != transpositionTable.end() && transpositionTable[boardHash].first >= beta) { //check if board state was evaluated before (from table)
        return transpositionTable[boardHash];
    }

    int gameisTerminal = inputBoard.gameisTerminal(inputBoard);
    std::vector<move> moves;
    if (gameisTerminal != 0) { // THEN GAME IS TERMINAL
        return std::make_pair(gameisTerminal, moves); // return utility and null
    }
    if (currentDepth == inputBoard.currentdepth) { // if depth limit is reached
        return std::make_pair(inputBoard.eval(inputBoard) + 1, moves);
    }
    int v = INT_MIN;
    std::vector<std::vector<move>> legalmoves = inputBoard.legalMoves(inputBoard);
    for (int i = 0; i < legalmoves.size(); i++) {
        board searchboard = inputBoard;

        searchboard.applyChoice(legalmoves[i], searchboard); // do move and then pass it to minValue for player 1 (user) turn
        std::pair<int,std::vector<move>> minvaluemove = minValue(searchboard, playerTurn, alpha, beta, currentDepth+1, transpositionTable);
        if (minvaluemove.first > v) {
            v = minvaluemove.first;
            moves = legalmoves[i];
            alpha = std::max(alpha,v);
        }
        if (v >= beta) {
            transpositionTable[boardHash] = std::make_pair(v, moves); //save evaluation and moveset to table
            return std::make_pair(v, moves);
        }
    }
    transpositionTable[boardHash] = std::make_pair(v, moves); //save evaluation and moveset to table
    return std::make_pair(v,moves);
}
        
std::pair<int,std::vector<move>> board::minValue(board& inputBoard, bool playerTurn, int alpha, int beta, int currentDepth, std::unordered_map<std::string, std::pair<int, std::vector<move>>>& transpositionTable) {
    if (!inputBoard.computergame) {
        inputBoard.playerTurn = true; //min value is always going to be player 1 (user)
    }
    
    std::string boardHash = generateHash(inputBoard);

    if (transpositionTable.find(boardHash) != transpositionTable.end() && transpositionTable[boardHash].first <= alpha) { //check if board state was evaluated before (from table)
        return transpositionTable[boardHash];
    }

    int gameisTerminal = inputBoard.gameisTerminal(inputBoard);
    std::vector<move> moves;
    if (gameisTerminal != 0) { // THEN GAME IS TERMINAL
        return std::make_pair(gameisTerminal, moves); // return utility and null
    }
    if (currentDepth == inputBoard.currentdepth) { //if depth limit is reached
        return std::make_pair(-inputBoard.eval(inputBoard) - 1, moves);
    }
    int v = INT_MAX;
    std::vector<std::vector<move>> legalmoves = inputBoard.legalMoves(inputBoard);
    for (int i = 0; i < legalmoves.size(); i++) {
        board searchboard = inputBoard;

        searchboard.applyChoice(legalmoves[i], searchboard); // do move and then pass it to maxValue for player 2 (computer) turn
        std::pair<int,std::vector<move>> maxvaluemove = maxValue(searchboard, playerTurn, alpha, beta, currentDepth+1, transpositionTable);
        if (maxvaluemove.first < v) {
            v = maxvaluemove.first;
            moves = legalmoves[i];
            beta = std::min(beta,v);
        }
        if (v <= alpha) {
            transpositionTable[boardHash] = std::make_pair(v, moves); //save evaluation and moveset to table
            return std::make_pair(v, legalmoves[i]);
        }
    }
    transpositionTable[boardHash] = std::make_pair(v, moves); //save evaluation and moveset to table
    return std::make_pair(v,moves);
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
    eval = manCount + kingCount * 5/3; //evaluation function counting the number of men and kings (difference between the two players)
    return eval;
}

int board::gameisTerminal(board& inputBoard) {
    board termgame = inputBoard;
    if (termgame.playerTurn) { //for minvalue function
        std::vector<std::vector<move>> legalmovesPlayer1 = legalMoves(termgame);
        bool playerTurn = termgame.playerTurn;
        termgame.playerTurn = !playerTurn;
        std::vector<std::vector<move>> legalmovesPlayer2 = legalMoves(termgame);
        if (legalmovesPlayer1.empty()) { // user loses
            return 999999;
        } 
        if (legalmovesPlayer2.empty()) { // computer loses            
            return -999999;
        }
        return 0; 
    } else { //for max value function
        std::vector<std::vector<move>> legalmovesPlayer2 = legalMoves(termgame);
        bool playerTurn = termgame.playerTurn;
        termgame.playerTurn = !playerTurn;
        std::vector<std::vector<move>> legalmovesPlayer1 = legalMoves(termgame);
        if (legalmovesPlayer1.empty()) { // compter loses
            return 999999;
        } 
        if (legalmovesPlayer2.empty()) { //user loses
            return -999999;
        }
        return 0;
    }
}

std::vector<std::vector<move>> board::legalMoves(board& inputBoard) {
    std::vector<std::vector<move>> legalmoves;
    std::vector<move> moveset;
    std::vector<move> jumpmoves;
    std::vector<move> regmoves;
    move firstmove;
    jumpmoves = checkJump(inputBoard, -1, 0); //check for jump moves
    if (jumpmoves.empty()) { // then there are no jumps available
        regmoves = checkRegular(inputBoard);
        for (int i = 0; i < regmoves.size(); i++) {
            moveset.emplace_back(regmoves[i]);
            legalmoves.emplace_back(moveset);
            moveset.clear();
        }
        return legalmoves; //returns just regular moves
    }
    if (jumpmoves.size() > 1) { //this series of statements is to convert the vector<move>jupmoves to the vector<vector<move>> legalmoves
        firstmove = jumpmoves[0]; //jump moves (by jump number) are ordered. example: 0 1 2 3 1 1 0 1 2 3 4 3 4 5 (because it was found recursively)
        moveset.emplace_back(firstmove);                                 // this converts to: 0 1 2 3 0 1 0 1 0 1 2 3 4 0 1 2 3 4 5 (so for example, 0->3) is a moveset in legalmoves in position i
        for (int i = 1; i < jumpmoves.size(); i++) {
            if (jumpmoves[i].jumpnumber == 0) {
                legalmoves.emplace_back(moveset);
                moveset.clear();
                firstmove = jumpmoves[i];
                moveset.emplace_back(firstmove);
            } else if(jumpmoves[i].jumpnumber > jumpmoves[i-1].jumpnumber) {
                moveset.emplace_back(jumpmoves[i]);
            } else if (jumpmoves[i].jumpnumber == jumpmoves[i-1].jumpnumber) {
                legalmoves.emplace_back(moveset);
                moveset.pop_back();
                moveset.emplace_back(jumpmoves[i]);
            } else if (jumpmoves[i].jumpnumber < jumpmoves[i-1].jumpnumber) {
                legalmoves.emplace_back(moveset);
                int numberofpops = jumpmoves[i-1].jumpnumber - jumpmoves[i].jumpnumber;
                for (int i = 0; i < numberofpops + 1; i++) {
                    moveset.pop_back();
                }
                moveset.emplace_back(jumpmoves[i]);
            } else {
                legalmoves.emplace_back(moveset);
                moveset.clear();
                moveset.emplace_back(firstmove);
                moveset.emplace_back(jumpmoves[i]);
            }
        }
        if (!moveset.empty()) {
            legalmoves.emplace_back(moveset);
        }
    }
    if (jumpmoves.size() == 1) { //if there's just one jump (and also one move) to be made
        firstmove = jumpmoves[0];
        moveset.emplace_back(firstmove);
        legalmoves.emplace_back(moveset);
    }
    return legalmoves;
}

void board::applyChoice(std::vector<move> moveset, board& inputBoard) {
    if (moveset[0].jumpnumber == -1) { //regular move
        movePiece(moveset[0], inputBoard);
    } else {
        for (int i = 0; i < moveset.size(); i++) { //jump moves
            movePiece(moveset[i], inputBoard);
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

std::vector<move> board::checkRegular(board& inputBoard) { //regular one diagonal moves
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
    if (position > 0) { // this if statement goes off it is a consecutive move (not the first jump)
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
                            for (int i = 0; i < returnedjumpmoves.size(); i++) {
                                jumpmoves.push_back(returnedjumpmoves[i]);
                            }
                        }
                        jumpboard = inputBoard;
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
                            for (int i = 0; i < returnedjumpmoves.size(); i++) {
                                jumpmoves.push_back(returnedjumpmoves[i]);
                            }
                        }
                        jumpboard = inputBoard;
                    }
                }
            }
        }
    } else { // otherwise check for first jumps
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
                                for (int i = 0; i < returnedjumpmoves.size(); i++) {
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
                                for (int i = 0; i < returnedjumpmoves.size(); i++) {
                                    jumpmoves.push_back(returnedjumpmoves[i]);
                                }
                            }
                            jumpboard = inputBoard;
                        }
                    }
                }
            }
        }
    }
    return jumpmoves;
}

void board::loadBoard(std::vector<int>& loadBoardArray, board& inputBoard) {
    for (int i = 0; i < 32; i++) {
        inputBoard.boardArray[i] = loadBoardArray[i];
    }
}

void board::showBoard(board& inputBoard) {
    std::cout << std::endl << "+----+----+----+----+----+----+----+----+" << std::endl;

    for (int i = 0; i < sizeof(inputBoard.boardArray) / sizeof(inputBoard.boardArray[0]); i++) {
        std::string piece;
        std::string color;

        switch (inputBoard.boardArray[i]) {
            case 0: // Empty
                color = "";
                piece = "  ";
                break;
            case 1: // Black Pawn
                color = "\033[34m";
                piece = "B ";
                break;
            case 2: // Red Pawn
                color = "\033[31m";
                piece = "R ";
                break;
            case 3: // Black King
                color = "\033[34m";
                piece = "BK";
                break;
            case 4: // Red King
                color = "\033[31m";
                piece = "RK";
                break;
            default: // Error
                piece = " ?";
                break;
        }

        int row = i / 4;
        bool skipFirst = row % 2 == 0;
        bool lastCell = (i + 1) % 4 == 0;

        if (skipFirst) {
            std::cout << "|    |" << " " << color << piece << "\033[0m" << + " ";
        } else {
            std::cout << "| " << color << piece << "\033[0m" << " |" << "    ";
        }

        if (lastCell) {
            std::cout << "|";
            std::cout << std::endl << "+----+----+----+----+----+----+----+----+" << std::endl;
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
        inputBoard.time = std::stoi(line);

        inputFile.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
}