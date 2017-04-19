#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>
#include <chrono>

#include "type.h"

class Timer {

public:
    explicit Timer(bool run) {
        if (run) {
            reset();
        }
    }

    void reset();
    double elapsed();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

// Performance measurement
double getElapsedTime(std::chrono::time_point<std::chrono::high_resolution_clock> startTime);

// Parsing
char getOppSide(char side);
std::vector<Move> split(const std::string&, char delim);

// Input / Output
void printResult(std::vector<Move> bestMoves, int numNodes, int depth, double elapsedTime);

#endif
