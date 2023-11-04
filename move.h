#include <vector>

#ifndef MOVE_H
#define MOVE_H

class move {
    public:
        int position;
        int destination;
        bool jump;
        int jumpedover;
        bool promote;
        bool player;
        bool firstjump;
        int jumpnumber;

        move() : position(-1), destination(-1), jump(false), jumpedover(-1), promote(false), player(true), firstjump(false), jumpnumber(-1) {}

        move(int positionInput, int destinationInput,int jumpedoverInput, bool promoteInput, bool playerInput, bool jumpInput, bool firstjumpInput, int jumpnumberInput) {
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
