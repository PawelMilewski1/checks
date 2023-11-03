#include <vector>

class move {
    public:
        int position;
        int destination;
        bool jump;
        int removedPos;
        bool reachedEnd;
        bool player;

        move(int positionInput, int destinationInput,int removedPosInput, bool reachedEndInput, bool playerInput) {
            position = positionInput;
            destination = destinationInput;
            jump = true;
            removedPos = removedPosInput;
            reachedEnd = reachedEndInput;
            player = playerInput;
        }
};