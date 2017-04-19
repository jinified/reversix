#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <omp.h>

#include "type.h"
#include "utils.h"
#include "evaluate.h"
#include "negascout.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "USAGE: ./reversi <board file> <evaluation params file>\n");
        return EXIT_FAILURE;
    }
    GameInfo gameInfo(argv[1]);
    gameInfo.show();
    EvalInfo evalInfo(argv[2]);
    evalInfo.show();

    Board *initialBoard = new Board(gameInfo.row, gameInfo.col);
    initialBoard->fillBoard('w', gameInfo.whitePosList);
    initialBoard->fillBoard('b', gameInfo.blackPosList);
    vector<Move> moves = initialBoard->getLegalMoves('b');

    // Initial board condition
    Timer timer(true);
    MoveScore bestMove = negascout(initialBoard, 0, 8, -INF, INF, gameInfo.side);
    printf("Elapsed: %f\n", timer.elapsed());

    // After move
    delete initialBoard;
    return 0;
}
