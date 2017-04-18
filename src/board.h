#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <cstddef>

#include "utils.h"

#define MAX_BOARD_SIZE 676

using namespace std;

class Board {

private:
    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);

public:
    Board(int, int);
    ~Board();
    vector<int> black = vector<int>(MAX_BOARD_SIZE);
    vector<int> taken = vector<int>(MAX_BOARD_SIZE);
    int row;
    int col;
    int size;

    // Evaluation function parameters
    int edge_weight = 5;
    int corner_weight = 50;
    int moves_weight =2;
    int diagonal_weight = -10;
    int frontier_weight = 2;

    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    vector<Move> getMoves(Side side);
    void doMove(Move *m, Side side);
    void setupBoard(char*, char*);
    int count(Side side);
    int countBlack();
    int countWhite();

    // Evaluation functions
    int getScore(Side maximizer);
    int boolToInt(bool b);
    int getMovesScore(Side maximizer);
    int getFrontierScore(Side maximizer);

    // Visualization
    void printBoard(Side);
    void visualizeMoves(Side);
};

#endif
