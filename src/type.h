#ifndef __TYPE_H__
#define __TYPE_H__

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#define MAX_POSLIST 2704
#define INF 9999999


/*
 * Represents a legal move in Othello
*/
struct Move {
    int x = -1;
    int y = -1;
    Move() {};
    Move(const std::string  &str);
    Move(int x, int y);

    // Convert back to game notation
    std::string toString();
};

struct MoveScore {
    Move move;
    int score;
    MoveScore(Move move, int score);
};

struct Board {
    char **board;
    int maxRow;
    int maxCol;

    Board(int row, int col);
    ~Board();
    Board *copy();
    void fillBoard(char side, std::vector<Move> moves);

    char getOppSide(char side);
    bool onBoard(int row, int col);
    bool isExist(char side, int row, int col);
    bool isLegalMove(char side, const Move& m);
    std::vector<Move> getLegalMoves(char side);
    void doMove(char side, const Move& m);

    void printBoard();
};

std::vector<Move> getOrderedMoves(char side, Board *board);

/*
 * Describes parameters of evaluation function
*/
struct EvalInfo {
    int maxDepth;
    int maxNodes;
    int cornerValue;
    int edgeValue;

    EvalInfo(char *filename);
    void show();
};

/*
 * Describes configuration of the game
 * side: indicates the color we are playing as
 * timeout: maximum duration in seconds allowed for computation
*/
struct GameInfo {
   int row;
   int col;
   int timeout;
   char side;
   std::vector<Move> whitePosList;
   std::vector<Move> blackPosList;

   GameInfo(char *filename);
   void show();
};

/*
 * Computation result from one computation unit
*/
struct Result {
    double time;        // Time elapsed for the particular computation
    int nodes;          // Number of nodes explored
    int depth;          // Maximum depth explored
    Move bestMove;      // Recommended move
};

#endif
