/*
Pawel Milewski - Fall 2023
This file includes variables and functions for class "move"
Individual moves are stored (NOT MOVESETS) in this class
*/

#include <vector>

#ifndef MOVE_H
#define MOVE_H

class move {
    public:
        int position; //move starting position
        int destination; //move ending position, either one diagonal, or two diagonals if a jump
        bool jump; //true if jump move
        int jumpedover; //one diagonal next to starting position (where the enemy was that was jumped over)
        bool promote; //if reached end of board
        bool player; //which player it belongs to
        bool firstjump; 
        int jumpnumber; // which jump in a list of jumps (0 = first jump, 1 = second jump and so on) (USED TO SORT THE MOVES ONCE ALL IN ONE ARRAY)

        move() : position(-1), destination(-1), jump(false), jumpedover(-1), promote(false), player(true), firstjump(false), jumpnumber(-1) {} //default vals

        move(int positionInput, int destinationInput,int jumpedoverInput, bool promoteInput, bool playerInput, bool jumpInput, bool firstjumpInput, int jumpnumberInput) { //creating object
            position = positionInput;
            destination = destinationInput;
            jump = jumpInput;
            jumpedover = jumpedoverInput;
            promote = promoteInput;
            player = playerInput;
            firstjump = firstjumpInput;
            jumpnumber = jumpnumberInput;
        }
};

#endif // MOVE_H
