#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "type.h"
#include "utils.h"

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
    return 0;
}
