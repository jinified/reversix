#include <vector>

#include "negascout.h"
#include "type.h"
#include "evaluate.h"

using namespace std;

/**
 * Calculate best move given a particular game state
*/
MoveScore negascout(Board *board, int depth, int maxDepth, int alpha, int beta,
               char side) {
    // Initialization

    char oppPiece = side == 'w' ? 'b' : 'w';
    Move bestMove;
    int currentScore;
    int bestScore = -INF;
    int adaptiveBeta = beta;

    // Terminates and return current best move if exceeded maximum depth
    if (depth > maxDepth) {
        return MoveScore(Move(), getScore(side, board));
    }

    // Generate moves according to evaluation function
    vector<Move> moveList = getOrderedMoves(side, board);

    if (moveList.empty()) {
        return MoveScore(Move(), bestScore);
    }

    bestMove = moveList[0];
    for (Move currentMove: moveList) {
        Board *newBoard = board->copy();
        newBoard->doMove(side, currentMove);

        // Recursion
        MoveScore current = negascout(newBoard, depth + 1, maxDepth, - adaptiveBeta,
                                      - max(alpha, bestScore), oppPiece);
        
        currentScore = current.score;

        // Update best score
        if (currentScore > bestScore) {
            // Regular AB search for narrow range
            if (adaptiveBeta == beta || depth >= (maxDepth - 2)) {
                bestScore = currentScore;
                bestMove = currentMove;
            } else {
                current = negascout(newBoard, depth+1, maxDepth, -beta, -currentScore, oppPiece);
                bestMove = currentMove;
            }

            // Pruning when out of bound
            if (bestScore >= beta) {
                delete newBoard;
                return MoveScore(bestMove, bestScore);
            }

            adaptiveBeta = max(alpha, bestScore) + 1;
        }
        delete newBoard;
    }
    return MoveScore(bestMove, bestScore);
}
