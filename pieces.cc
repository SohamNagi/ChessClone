#include "pieces.h"
#include "coord.h"
#include <iostream>


// Still need to implement:
// - Remove moves that result in checks
// - Pawn promotions
// - En Passant
// - Castling
// - Remove all moves that don't fix a current check
// - should something else change if the legal move involves capturing?

// personal notes:

// Use functions for the bishop and rook so the queen's function is shorter
// + a lot could be done to make the code shorter and more readable

// could implement a vector in the board class that has the attack coords for white and black
// the only problem with this is the processing required, after every move the program would need to iterate through
// all 64 locations and iterate through those coords' legalmoves vector. Could instead copy over the entire vector,
// so instead we have 2 vectors (black and white attack coords) that consist of 0-16 vectors each, optimizing the program

// temporarily make text render function where you can select a square and see its legal moves (as + symbols)

// Piece constructor
Pieces::Pieces(Board* board, bool isWhite, int location, char type, bool isEmpty) :
    board{ board }, isWhite{ isWhite }, location{ location }, type{ type }, isEmpty{ isEmpty }, legalmoves{ } {}

// Piece destructor
Pieces::~Pieces() {}

// emptyPiece constructor
emptyPiece::emptyPiece(Board* board, bool isWhite, int location, char type) :
    Pieces{ board, isWhite, location, type, true } {}

// emptyPiece updatemoves
void emptyPiece::updateMoves() {}

// emptyPiece destructor
emptyPiece::~emptyPiece() {}

// Pawn constructor
Pawn::Pawn(Board* board, bool isWhite, int location, char type) :
    Pieces{ board, isWhite, location, type, false }, moved{ false } {}

void Pawn::updateMoves() {
    int x = location % 8;
    int y = (location-(location % 8)) / 8;

    // White Pawns:
    // Check the space immediately above
    if (isWhite && y + 1 <= 7 && board->boardState[(y + 1) * 8 + x]->isEmpty) {
        legalmoves.emplace_back((y + 1) * 8 + x);
    }
    // Check the space 2 above
    if (isWhite && !moved && board->boardState[(y + 2) * 8 + x]->isEmpty) {
        legalmoves.emplace_back((y + 2) * 8 + x);
    }
    // Check 1 up and 1 left to see if a capture is available
    if (isWhite && y + 1 <= 7 && x - 1 >= 0 && !board->boardState[(y + 1) * 8 + (x - 1)]->isWhite) {
        legalmoves.emplace_back((y + 1) * 8 + (x - 1));
    }
    // Check 1 up and 1 right to see if a capture is available
    if (isWhite && y + 1 <= 7 && x + 1 <= 7 && !board->boardState[(y + 1) * 8 + (x + 1)]->isWhite) {
        legalmoves.emplace_back((y + 1) * 8 + (x + 1));
    }

    // Black Pawns:
    // Check the space immediately below
    if (!isWhite && y - 1 >= 0 && board->boardState[(y - 1) * 8 + x]->isEmpty) {
        legalmoves.emplace_back((y - 1) * 8 + x);
    }
    // Check the space 2 below
    if (!isWhite && !moved && board->boardState[(y - 2) * 8 + x]->isEmpty) {
        legalmoves.emplace_back((y - 2) * 8 + x);
    }
    // Check 1 down and 1 left to see if a capture is available
    if (!isWhite && y - 1 >= 0 && x - 1 >= 0 && board->boardState[(y - 1) * 8 + (x - 1)]->isWhite) {
        legalmoves.emplace_back((y - 1) * 8 + (x - 1));
    }
    // Check 1 down and 1 right to see if a capture is available
    if (!isWhite && y - 1 >= 0 && x + 1 <= 7 && board->boardState[(y - 1) * 8 + (x + 1)]->isWhite) {
        legalmoves.emplace_back((y - 1) * 8 + (x + 1));
    }
}

// Pawn destructor
Pawn::~Pawn() {}

// Rook constructor
Rook::Rook(Board* board, bool isWhite, int location, char type) :
    Pieces{ board, isWhite, location, type, false }, moved{ false } {}

void Rook::updateMoves() {
    int x = location % 8;
    int y = (location-(location % 8)) / 8;

    // From the Rook to the top of the board
    for (int i = y; i < 7; i++) {
        if (board->boardState[i * 8 + x]->isEmpty) {
            legalmoves.emplace_back(i * 8 + x);
        } else if (board->boardState[i * 8 + x]->isWhite != isWhite) {
            legalmoves.emplace_back(i * 8 + x);
            break;
        } else break;
    }
    
    // From the Rook to the bottom of the board
    for (int i = y; i > 0; i--) {
        if (board->boardState[i * 8 + x]->isEmpty) {
            legalmoves.emplace_back(i * 8 + x);
        } else if (board->boardState[i * 8 + x]->isWhite != isWhite) {
            legalmoves.emplace_back(i * 8 + x);
            break;
        } else break;
    }

    // From the Rook to the right of the board
    for (int i = x; i > 7; i++) {
        if (board->boardState[y * 8 + i]->isEmpty) {
            legalmoves.emplace_back(y * 8 + i);
        } else if (board->boardState[y * 8 + i]->isWhite != isWhite) {
            legalmoves.emplace_back(y * 8 + i);
            break;
        } else break;
    }

    // From the Rook to the left of the board
    for (int i = x; i > 0; i--) {
        if (board->boardState[y * 8 + i]->isEmpty) {
            legalmoves.emplace_back(y * 8 + i);
        } else if (board->boardState[y * 8 + i]->isWhite != isWhite) {
            legalmoves.emplace_back(y * 8 + i);
            break;
        } else break;
    }

}

// Rook destructor
Rook::~Rook() {}

// King constructor
King::King(Board* board, bool isWhite, int location, char type) :
    Pieces{ board, isWhite, location, type, false }, moved{ false } {}

void King::updateMoves()  {
    int x = location % 8;
    int y = (location-(location % 8)) / 8;

    // Check all surrounding spaces for the King
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int possible = (y + j) * 8 + (x + i);
            if (0 <= possible && possible < 64 && !(i == 0 && j == 0) && (board->boardState[possible]->isEmpty ||
                !board->boardState[possible]->isWhite)) {
                    legalmoves.emplace_back((y + j) * 8 + (x + i));
                }
        }
    }
}

// King destructor
King::~King() {}

// Queen constructor
Queen::Queen(Board* board, bool isWhite, int location, char type) :
    Pieces{ board, isWhite, location, type, false } {}

void Queen::updateMoves() {
    int x = location % 8;
    int y = (location-(location % 8)) / 8;
    // Queen x/y sliding:    
    // From the Queen to the top of the board
    for (int i = y + 1; i >= 0 && i < 7; i++) {
        if (board->boardState[i * 8 + x]->isEmpty) {
            legalmoves.emplace_back(i * 8 + x);
        } else if (board->boardState[i * 8 + x]->isWhite != isWhite) {
            legalmoves.emplace_back(i * 8 + x);
            break;
        } else break;
    }
    // From the Queen to the bottom of the board
    for (int i = y + 1; i >= 0 && i < 7; i--) {
        if (board->boardState[i * 8 + x]->isEmpty) {
            legalmoves.emplace_back(i * 8 + x);
        } else if (board->boardState[i * 8 + x]->isWhite != isWhite) {
            legalmoves.emplace_back(i * 8 + x);
            break;
        } else break;
    }
    // From the Queen to the right of the board
    for (int i = x + 1; i >= 0 && i < 7; i++) {
        if (board->boardState[y * 8 + i]->isEmpty) {
            legalmoves.emplace_back(y * 8 + i);
        } else if (board->boardState[y * 8 + i]->isWhite != isWhite) {
            legalmoves.emplace_back(y * 8 + i);
            break;
        } else break;
    }
    // From the Queen to the left of the board
    for (int i = x + 1; i >= 0 && i < 7; i--) {
        if (board->boardState[y * 8 + i]->isEmpty) {
            legalmoves.emplace_back(y * 8 + i);
        } else if (board->boardState[y * 8 + i]->isWhite != isWhite) {
            legalmoves.emplace_back(y * 8 + i);
            break;
        } else break;
    }

    // Queen diagonal sliding
    bool doubleBreak = false;

    // Checking the up/right diagonal
    for (int i = x + 1; i < 8; i++) {
        for (int j = y + 1; j < 8; j++) {
            if (abs(i - x) == abs(j - y)) {
                if (board->boardState[j * 8 + i]->isWhite == isWhite) {
                    doubleBreak = true;
                    break;
                }
                legalmoves.emplace_back(j * 8 + i);
                if (board->boardState[j * 8 + i]->isWhite != isWhite) {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak) break;
    }

    doubleBreak = false;
    // Checking the down/right diagonal
    for (int i = x + 1; i < 8; i++) {
        for (int j = y - 1; j >= 0; j--) {
            if (abs(i - x) == abs(j - y)) {
                if (board->boardState[j * 8 + i]->isWhite == isWhite) {
                    doubleBreak = true;
                    break;
                }
                legalmoves.emplace_back(j * 8 + i);
                if (board->boardState[j * 8 + i]->isWhite != isWhite) {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak) break;
    }

    doubleBreak = false;

    // Checking the down/left diagonal
    for (int i = x - 1; i >= 0; i--) {
        for (int j = y - 1; j >= 0; j--) {
            if (abs(i - x) == abs(j - y)) {
                if (board->boardState[j * 8 + i]->isWhite == isWhite) {
                    doubleBreak = true;
                    break;
                }
                legalmoves.emplace_back(j * 8 + i);
                if (board->boardState[j * 8 + i]->isWhite != isWhite) {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak) break;
    }

    doubleBreak = false;

    // Checking the up/left diagonal
    for (int i = x - 1; i >= 0; i--) {
        for (int j = y + 1; j < 8; j++) {
            if (abs(i - x) == abs(j - y)) {
                if (board->boardState[j * 8 + i]->isWhite == isWhite) {
                    doubleBreak = true;
                    break;
                }
                legalmoves.emplace_back(j * 8 + i);
                if (board->boardState[j * 8 + i]->isWhite != isWhite) {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak) break;
    }
}

// Queen destructor
Queen::~Queen() {}

// Knight constructor
Knight::Knight(Board* board, bool isWhite, int location, char type) :
    Pieces{ board, isWhite, location, type, false } {}

void Knight::updateMoves() {
    int x = location % 8;
    int y = (location-(location % 8)) / 8;
    // Combining these two arrays gives the 8 possible moves for any knight
    int xChange[8] = {-1, 1, -2, -2, -1, 1, 2, 2};
    int yChange[8] = {-2, -2, -1, 1, 2, 2, -1, 1};

    // Looping through all 8 positions and checking if they are empty or capturable
    for (int i = 0; i < 8; i++) {
        int possible = (y + yChange[i]) * 8 + x + xChange[i];
        if (0 <= possible && possible < 64 && (board->boardState[possible]->isEmpty ||
            board->boardState[possible]->isWhite != isWhite)) {
                legalmoves.emplace_back(possible);
        }
    }
}

// Knight destructor
Knight::~Knight() {}

// Bishop constructor
Bishop::Bishop(Board* board, bool isWhite, int location, char type) :
    Pieces{ board, isWhite, location, type, false } {}

void Bishop::updateMoves() {
    int x = location % 8;
    int y = (location-(location % 8)) / 8;

    // create a helper function for the inner portion of the loop

    bool doubleBreak = false;

    // Checking the up/right diagonal
    for (int i = x + 1; i < 8; i++) {
        for (int j = y + 1; j < 8; j++) {
            if (abs(i - x) == abs(j - y)) {
                if (board->boardState[j * 8 + i]->isWhite == isWhite) {
                    doubleBreak = true;
                    break;
                }
                legalmoves.emplace_back(j * 8 + i);
                if (board->boardState[j * 8 + i]->isWhite != isWhite) {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak) break;
    }

    doubleBreak = false;

    // Checking the down/right diagonal
    for (int i = x + 1; i < 8; i++) {
        for (int j = y - 1; j >= 0; j--) {
            if (abs(i - x) == abs(j - y)) {
                if (board->boardState[j * 8 + i]->isWhite == isWhite) {
                    doubleBreak = true;
                    break;
                }
                legalmoves.emplace_back(j * 8 + i);
                if (board->boardState[j * 8 + i]->isWhite != isWhite) {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak) break;
    }

    doubleBreak = false;

    // Checking the down/left diagonal
    for (int i = x - 1; i >= 0; i--) {
        for (int j = y - 1; j >= 0; j--) {
            if (abs(i - x) == abs(j - y)) {
                if (board->boardState[j * 8 + i]->isWhite == isWhite) {
                    doubleBreak = true;
                    break;
                }
                legalmoves.emplace_back(j * 8 + i);
                if (board->boardState[j * 8 + i]->isWhite != isWhite) {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak) break;
    }

    doubleBreak = false;

    // Checking the up/left diagonal
    for (int i = x - 1; i >= 0; i--) {
        for (int j = y + 1; j < 8; j++) {
            if (abs(i - x) == abs(j - y)) {
                if (board->boardState[j * 8 + i]->isWhite == isWhite) {
                    doubleBreak = true;
                    break;
                }
                legalmoves.emplace_back(j * 8 + i);
                if (board->boardState[j * 8 + i]->isWhite != isWhite) {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak) break;
    }
}

// Bishop destructor
Bishop::~Bishop() {}
