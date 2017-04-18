#ifndef __TYPE_H__
#define __TYPE_H__

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#define MAX_POSLIST 2704

enum Side {
    BLACK, WHITE
};

/*
 * Represents a legal move in Othello
*/
struct Move {
    int x;
    int y;
    Move(const std::string  &str);
    Move(int x, int y);

    // Convert back to game notation
    std::string toString();
};

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
   Side side;
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
