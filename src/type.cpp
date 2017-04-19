#include <string>
#include <sstream>
#include <algorithm>
#include <omp.h>

#include "type.h"
#include "utils.h"
#include "evaluate.h"

using namespace std;

// Evaluator

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

   side = player[0] == 'W' ? 'w' : 'b';

   // Parse discs on the initial board
   whitePosList = split(string(whitePos), ',');
   blackPosList = split(string(blackPos), ',');
}

void GameInfo::show() {
    printf("\n=== Game Info ===\n"
           "Board dimension: %d,%d\n"
           "Side: %s\n"
           "Timeout: %d\n", row, col, side == 'w' ? "White" : "Black", timeout);
}

// Move

Move::Move(const std::string &str) {
    x  = (str[1] - '0') - 1;
    y  = str[0] - 'a';
}

Move::Move(int x, int y) {
    // x = row, y = col
    this->x = x;
    this->y = y;
}

string Move::toString() {
    char col = "abcdefghijklmnopqrstuvwxyz"[y];
    stringstream ss;
    ss << col << x + 1;
    return ss.str();
}

MoveScore::MoveScore(Move move, int score) {
    this->move = move;
    this->score = score;
}

// Board

Board::Board(int row, int col) {
    this->maxRow = row;
    this->maxCol = col;

    this->board = new char*[row];
    char* pool = new char[row * col];
    for (int i = 0; i < row; ++i, pool += col) {
        this->board[i] = pool;
    }

    for (int i=0; i < row; i++) {
        for (int j=0; j < col; j++) {
            this->board[i][j] = '-';
        }
    }
}

Board::~Board() {
    delete [] board[0];
    delete [] board;
}

Board* Board::copy() {
    Board *newBoard = new Board(maxRow, maxCol);
    # pragma omp parallel
    # pragma omp for
    for (int i=0; i < maxRow; i++) {
        for (int j=0; j < maxCol; j++) {
            newBoard->board[i][j] = this->board[i][j];
        }
    }
    return newBoard;
}

void Board::fillBoard(char side, vector<Move> moves) {
    for (Move m: moves) {
        this->board[m.x][m.y] = side;
    }
}

char Board::getOppSide(char side) {
    return side == 'w' ? 'b' : 'w';
}

bool Board::onBoard(int r, int c) {
    return(0 <= r && r < maxRow && 0 <= c && c < maxCol);
}

// Checks if the side's disc is on the position board[row][col]
bool Board::isExist(char side, int row, int col) {
    return onBoard(row, col) && this->board[row][col] == side;
}

bool Board::isLegalMove(char side, const Move& m) {
    // Important make sure don't mix up
    int X = m.x;
    int Y = m.y;
    char other = getOppSide(side);

    // Make sure the square hasn't already been taken.
    if (this->board[X][Y] != '-') {
        return false;
    }

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (isExist(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (isExist(other, x, y));

                if (isExist(side, x, y)) return true;
            }
        }
    }
    return false;
}

vector<Move> Board::getLegalMoves(char side) {
    char oppPiece = getOppSide(side);
    vector<Move> moves;
    for (int i=0; i < maxRow; i++) {
        for (int j=0; j < maxCol; j++) {
            Move m(i, j);
            if (isLegalMove(side, m)) {
                moves.push_back(m);
            }
        }
    }
    return moves;
}

void Board::doMove(char side, const Move &m) {
    // Null move
    if ((m.x == -1 && m.y == -1) || !isLegalMove(side, m)) {
        return;
    }
    int X = m.x;
    int Y = m.y;
    char other = getOppSide(side);

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (isExist(other, x, y));

            if (isExist(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (isExist(other, x, y)) {
                    this->board[x][y] = side;
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    this->board[X][Y] = side;
}

void Board::printBoard() {
    printf("\n  ");
    for (int c=0; c < maxCol; c++) {
        printf("%c ", "abcdefghijklmnopqrstuvwxyz"[c]);
    }
    printf("\n");

    for (int i=0; i < maxRow; i++) {
        printf("%d ", i + 1);
        for (int j=0; j < maxCol; j++) {
            printf("%c ", this->board[i][j]);
        }
        printf("\n");
    }
}

// General

vector<Move> getOrderedMoves(char side, Board *board) {
    vector<Move> moves = board->getLegalMoves(side);

    std::sort(moves.begin(), moves.end(), [&] (Move m1, Move m2)
        {
            Board *b1 = board->copy();
            b1->doMove(side, m1);
            Board *b2 = board->copy();
            b2->doMove(side, m2);
            int m1_score = getScore(side, b1);
            int m2_score = getScore(side, b2);
            delete b1;
            delete b2;
            return m1_score > m2_score;
        }
    );
    return moves;
}
