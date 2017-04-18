#include <string>
#include <sstream>

#include "type.h"
#include "utils.h"

using namespace std;

EvalInfo::EvalInfo(char *filename) {
   std::ifstream input(filename);

   for (std::string line; getline(input,line);) {
       sscanf(line.c_str(), "MaxDepth: %d %*s", &maxDepth);
       sscanf(line.c_str(), "MaxBoards: %d %*s", &maxNodes);
       sscanf(line.c_str(), "CornerValue: %d %*s", &cornerValue);
       sscanf(line.c_str(), "EdgeValue: %d %*s", &edgeValue);
   }
}

void EvalInfo::show() {
    printf("\n=== Eval Info ===\n"
           "Max Depth: %d\n"
           "Max Nodes: %d\n"
           "Corner Value: %d\n"
           "Edge Value: %d\n", maxDepth, maxNodes, cornerValue, edgeValue);
}

// Game Info

GameInfo::GameInfo(char *filename) {
   std::ifstream input(filename);
   char whitePos[MAX_POSLIST];
   char blackPos[MAX_POSLIST];
   char player[MAX_POSLIST];

   for (std::string line; getline(input,line);) {
       sscanf(line.c_str(), "Size: %d,%d %*s", &row, &col);
       sscanf(line.c_str(), "White: { %s } %*s", whitePos);
       sscanf(line.c_str(), "Black: { %s } %*s", blackPos);
       sscanf(line.c_str(), "Color: %s %*s", player);
       sscanf(line.c_str(), "Timeout: %d %*s", &timeout);
   }

   side = player[0] == 'W' ? WHITE : BLACK;

   // Parse discs on the initial board
   whitePosList = split(string(whitePos), ',');
   blackPosList = split(string(blackPos), ',');
}

void GameInfo::show() {
    printf("\n=== Game Info ===\n"
           "Board dimension: %d,%d\n"
           "Side: %s\n"
           "Timeout: %d\n", row, col, side == WHITE ? "White" : "Black", timeout);
}

// Move

Move::Move(const std::string &str) {
    x  = str[0] - 'a';
    y  = (str[1] - '0') - 1;
}

Move::Move(int x, int y) {
    this->x = x;
    this->y = y;
}

string Move::toString() {
    char col = "abcdefghijklmnopqrstuvwxyz"[x-1];
    stringstream ss;
    ss << col << y;
    return ss.str();
}
