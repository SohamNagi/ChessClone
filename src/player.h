#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "coord.h"
#include "board.h"

class Rook;

class Player {

    public:
        bool isWhite;
        Board* board;
        Player(bool isWhite, Board* board);
        virtual ~Player() = 0;

        void resign();
        bool move(int start, int end, Board* board);
        virtual void getmove(Board* test = nullptr) = 0;
        virtual void promote(int piece) = 0;
        void castle(Rook* rook);
};


#endif


