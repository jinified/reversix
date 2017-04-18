#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>

#include "type.h"

// Parsing
std::vector<Move> split(const std::string&, char delim);
Move strtomove(const std::string &);

// Input / Output
void printResult(std::vector<Move> bestMoves, int numNodes, int depth, double elapsedTime);

#endif
