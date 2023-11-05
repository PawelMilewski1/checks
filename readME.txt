Pawel Milewski
AI Fall 2023
Checkers Submission

Write-up describing your program's implementation, features, and whatever else you would like me to know:

Compiling:
1. Copy/move all files to one folder (main.cpp, board.cpp, move.h, board.h, CMakeLists.txt)
2. I created a build folder within the main folder to keep files organized.
3. In the terminal and within the build folder, type "cmake .." to run the cmake file in the main folder
4. Then, in the build folder tpye "make" and then run the program with ./CheckersGame

Program Inputs:
When starting the program, the user is asked whether to load a game state or not which is an input file such as sampleCheckers1.txt. If there is no input game startTime, the user is asked for a time limit. The user is then asked if the game should boot in computer vs. computer mode which puts two computers against each other that use the same evaluation function. Otherwise the user plays as player 1 against the computer.

Move Choices:
Move choices are shown as INSERT_INITIAL_POSITION " TO " INSERT_FINAL_POSITION for both regular moves and jump moves. Jump moves are two diagonals away. The numbers are according to a "numbered checkers board" which seemed to be standard checkers notation. 
    0       1       2       3
4       5       6       7   
    8       9       10      11
12      13      14      15
    16      17      18      19
20      21      22      23
    24      25      26      27
28      29      30      31

Legalmoves:
Legalmoves were found by first checking if there were any available jump moves followed by checking for regular diagonals. Checking for available jump moves was a recursive function as to check for multiple jump moves. Jump moves then returned the list of moves, but it did not have them ordered properly. Jump numbers were assigned to individual moves (0 for first jump, 1 for second jump and so on). An example output of jump moves could be '0 1 2 3 2 3 3 4 1' <- these are just the jump numbers of the move objects returned. The legalmoves function then sorts this so that it becomes a vector of vectors of moves. The vectors of moves are the movesets (multiple consecutive jumps).
So from '0 1 2 3 2 3 3 4 1'
legal moves becomes 0   0   0   0
                    1   1   1   1
                    2   2   2
                    3   3   3
                            4
note that these are just jump numbers not the jump objects. if there's multiple first jumps there would then be more numbers after '0 1 2 3 2 3 3 4 1', starting with a zero. This is just added to the previous legalmoves vector of vectors, it's just that the first jump move is now different.

AI portion:
The AI portion features iterative deepening which makes use of an alphabetasearch that uses maxValue and minValue functions. Memory is retained through the use of a transposition table. 

Eval Function:
The evaluation function is simply finding the difference between the sum of the number of the men and kings between the two players. The number of kings is multiplied by a factor of 5/3 to account for the kings extra diagonals.