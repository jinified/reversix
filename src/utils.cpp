#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

#include "utils.h"
#include "type.h"

using namespace std;

vector<Move> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<Move> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(Move(item));
    }
    return tokens;
}

void printResult(vector<Move> bestMoves, int numNodes, int depth, double elapsedTime) {
    stringstream suggestedMoves;

    if (bestMoves.empty()) {
        suggestedMoves << "n/a";
    } else {
        suggestedMoves << bestMoves[0].toString();

        for (auto m = bestMoves.begin() + 1; m != bestMoves.end(); ++m) {
            suggestedMoves << "," << m->toString();
        }
    }

    printf("Best moves: { %s }\n"
           "Number of boards assessed: %d\n"
           "Depth of boards: %d\n"
           "Entire space: %s\n"
           "Elapsed time in seconds: %f\n", suggestedMoves.str().c_str(), numNodes, depth, "False",
           elapsedTime / 1000);
}
