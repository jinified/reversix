#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <type_traits>

#include "utils.h"
#include "type.h"

using namespace std;

/*Calculates elapsed time using the high resolution clock
 * Returns elapsed time in millisecond (ms)
*/
double getElapsedTime(chrono::time_point<chrono::high_resolution_clock> startTime) {
    auto finish = chrono::high_resolution_clock::now(); 
    chrono::duration<double> elapsed = finish - startTime;
    double elapsedTime = elapsed.count() * 1000;
    return elapsedTime;
}

vector<Move> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<Move> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(Move(item));
    }
    return tokens;
}

char getOppSide(char side) {
    return side == 'w' ? 'b' : 'w';
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

// Timer implementation

void Timer::reset() {
    start = chrono::high_resolution_clock::now();
}

double Timer::elapsed() {
    chrono::duration<double> elapsed = chrono::high_resolution_clock::now() - start;
    return elapsed.count() * 1000;
}
