#include "board.h"
#include <string.h>

/*
 * Make a row x col othello board
 */
Board::Board(int row, int col) {
    // Initialization
    this->row = row;
    this->col = col; 
    this->size = row * col;

    // Resizes vector to given board dimension
    black.resize(this->size);
    taken.resize(this->size);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board(row, col);
    for (int i = 0; i < this->size; i++) {
        newBoard->black[i] = black[i];
        newBoard->taken[i] = taken[i];
    }
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + row * y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + row * y] == boolToInt(side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken[x + row * y] = 1;
    // if side is black, side == BLACK will evaluate to 1
    black[x + row * y] = boolToInt(side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < col && 0 <= y && y < row);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            Move move(i,j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

// Returns a list of possible moves for the specified side
vector<Move> Board::getMoves(Side side) {
    vector<Move> movesList;
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) {
                movesList.push_back(move);
            }
        }
    }
    return movesList;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->x;
    int Y = m->y;

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->x;
    int Y = m->y;
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    int count = 0;
    for (int i = 0; i < this->size; i++) {
        count += black[i];
    }
    return count;
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    int count = 0;
    for (int i = 0; i < this->size; i++) {
        count += taken[i];
        count -= black[i];
    }
    return count;
}

/* 
 * Return the score of this board state for the maximizer.
 */
int Board::getScore(Side maximizer) {
    Side minimizer = maximizer == BLACK ? WHITE : BLACK;
    float score;

    if (maximizer == BLACK) {
        score = countBlack() - countWhite();
    } else {
        score = countWhite() - countBlack();
    }

    // update score by adding a positive weight if the maximizer has occupied a
    // corner or a negative weight if the minimizer has occupied a corner
    bool maxULCorner = get(maximizer, 0, 0);
    bool maxURCorner = get(maximizer, col - 1, 0);
    bool maxLLCorner = get(maximizer, 0, row - 1);
    bool maxLRCorner = get(maximizer, col - 1, row - 1);

    bool minULCorner = get(minimizer, 0, 0);
    bool minURCorner = get(minimizer, col - 1, 0);
    bool minLLCorner = get(minimizer, 0, row - 1);
    bool minLRCorner = get(minimizer, col - 1, col - 1);

    if (maxULCorner || maxURCorner || maxLLCorner || maxLRCorner) {
        score += corner_weight * (boolToInt(maxULCorner) + boolToInt(maxURCorner) 
                                + boolToInt(maxLLCorner) + boolToInt(maxLRCorner));
    }
    if (minULCorner || minURCorner || minLLCorner || minLRCorner) {
        score -= corner_weight * (boolToInt(minULCorner) + boolToInt(minURCorner) 
                                + boolToInt(minLLCorner) + boolToInt(minLRCorner));
    }

    // update score using a negative weight for positions in the diagonal that are
    // next to unoccupied corners
    bool maxULDiagonal = get(maximizer, 1, 1);
    bool maxURDiagonal = get(maximizer, col - 2, 1);
    bool maxLLDiagonal = get(maximizer, 1, row - 2);
    bool maxLRDiagonal = get(maximizer, col - 2, row - 2);

    if (maxULDiagonal && !minULCorner) {
        score += diagonal_weight;
    }
    if (maxURDiagonal && !minURCorner) {
        score += diagonal_weight;
    }
    if (maxLLDiagonal && !minLLCorner) {
        score += diagonal_weight;
    }
    if (maxLRDiagonal && !minLRCorner) {
        score += diagonal_weight;
    }

    // update score using a positive weight for occupied edge positions (edge
    // positions do not include the corners)
    for (int x = 0; x < col; x += col - 1) {
        for (int y = 1; y < row - 1; y++) {
            score += edge_weight * boolToInt(get(maximizer, x, y));
        }
    }
    for (int y = 0; y < row; y += row - 1) {
        for (int x = 1; x < col - 1; x++) {
            score += edge_weight * boolToInt(get(maximizer, x, y));
        }
    }

    score += moves_weight * getMovesScore(maximizer);
    score += frontier_weight * getFrontierScore(maximizer);

    return score;
 }


void Board::setupBoard(char *white_pos, char *black_pos) {
    // Sets board given positions for each player

    char *pt = strtok(white_pos, ",");
    while(pt != NULL) {
        int x = (*pt - 'a');
        int y = (*(pt+1) - '0') - 1;
        taken[x + row * y] = 1;
        pt = strtok (NULL, ",");
    }

    pt = strtok(black_pos, ",");
    while(pt != NULL) {
        int x = (*pt - 'a');
        int y = (*(pt+1) - '0') - 1;
        taken[x + row * y] = 1;
        black[x + row * y] = 1;
        pt = strtok (NULL, ",");
    }
}

int Board::boolToInt(bool b) {
    return b ? 1 : 0;
}

int Board::getMovesScore(Side maximizer) {
    Side minimizer = maximizer == BLACK ? WHITE : BLACK;
    vector<Move> moves = getMoves(maximizer);
    vector<Move> opponentMoves = getMoves(minimizer);
    return moves.size()-opponentMoves.size();
}

int Board::getFrontierScore(Side maximizer) {
    int score = 0;
    bool frontier = false;

    for (int x = 1; x < col - 1; x++) {
        for (int y = 1; y < row - 1; y++) {
            // check to see if position on the board is occupied by
            // either the maximizer or minimizer
            if (occupied(x, y)) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        // continue since it's the current position
                        // being checked
                        if (dx == 0 && dy == 0) {
                            continue;
                        }
                        // set flag since we have found an unoccupied
                        // position surrounding the current position
                        if (!occupied(x+dx, y+dy)) {
                            frontier = true;
                        }
                    }
                }
                if (frontier) {
                    if (get(maximizer, x, y)) {
                        // add to the score if maximizer is in the frontier
                        score--;
                    } else {
                        // subtract from the score if the minimizer is in
                        // the frontier
                        score++;
                    }
                }
            }
            frontier = false;
        }
    }
    return score;
}

// Visualizations

void Board::printBoard(Side side) {
    printf("\n");
    for (int y = 0; y < row; y++) {
        for (int x = 0; x < col; x++) {
            printf("%d ", get(side, x, y));
        }
        printf("\n");
    }
}


void Board::visualizeMoves(Side side) {
    vector<Move> moves = getMoves(side);
    Board *newboard = copy();
    for (Move m: moves) {
        newboard->doMove(&m, side);
    }
    newboard->printBoard(side);
}
