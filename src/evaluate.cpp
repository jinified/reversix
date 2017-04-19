#include <vector>

#include "evaluate.h"
#include "utils.h"

using namespace std;

int getStabilityScore(char side, Board *board, int weight) {
    char other = getOppSide(side);

    int maxRow = board->maxRow;
    int maxCol = board->maxCol;
    char **curr = board->board;

    int ourCorners = (curr[0][0] == side) + (curr[maxRow-1][0] == side)
        + (curr[0][maxCol-1] == side) + (curr[maxRow-1][maxCol-1] == side);

    int oppCorners = (curr[0][0] == other) + (curr[maxRow-1][0] == other)
        + (curr[0][maxCol-1] == other) + (curr[maxRow-1][maxCol-1] == other);

    return weight * (ourCorners - oppCorners);
}

int getMobilityScore(char side, Board *board) {
    char other = getOppSide(side);

    vector<Move> ourMoves = board->getLegalMoves(side);
    vector<Move> oppMoves = board->getLegalMoves(other);
    return ourMoves.size() - oppMoves.size();
}

int getEdgeScore(char side, Board *board, int weight) {
    char **curr = board->board;
    int row = board->maxRow;
    int col = board->maxCol;
    int score = 0;

    for (int x = 0; x < col; x += col - 1) {
        for (int y = 1; y < row - 1; y++) {
            score += weight * (curr[x][y] == side);
        }
    }
    for (int y = 0; y < row; y += row - 1) {
        for (int x = 1; x < col - 1; x++) {
            score += weight * (curr[x][y] == side);
        }
    }
    return score;
}

int getFrontierScore(char side, Board *board) {
    char **curr = board->board;
    int col = board->maxCol;
    int row = board->maxRow;
    int score = 0;
    bool frontier = false;

    for (int x = 1; x < row - 1; x++) {
        for (int y = 1; y < col - 1; y++) {
            // check to see if position on the board is occupied by
            // either the maximizer or minimizer
            if (curr[x][y] != '-') {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        // continue since it's the current position
                        // being checked
                        if (dx == 0 && dy == 0) {
                            continue;
                        }
                        // set flag since we have found an unoccupied
                        // position surrounding the current position
                        if (curr[x+dx][y+dy] == '-') {
                            frontier = true;
                        }
                    }
                }
                if (frontier) {
                    if (curr[x][y] == side) {
                        // subtract from the score if the minimizer is in
                        // the frontier
                        score--;
                    } else {
                        // add to the score if opponent is in the frontier
                        score++;
                    }
                }
            }
            frontier = false;
        }
    }
    return score;
}

int getScore(char side, Board *board) {
    int ourDiscs = 0;
    int oppDiscs = 0;

    for (int i=0; i < board->maxRow; i++) {
        for (int j=0; j < board->maxCol; j++) {
            char val = board->board[i][j];
            if (val != '-') {
                val == side ? ourDiscs ++ : oppDiscs++;
            }
        }
    }
    int score = ourDiscs - oppDiscs + getMobilityScore(side, board)
        + getStabilityScore(side, board) + getEdgeScore(side, board)
        + getFrontierScore(side, board);
}
