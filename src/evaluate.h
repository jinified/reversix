#ifndef __EVALUATE_H__
#define __EVALUATE_H__

#define EDGE_WEIGHT 4
#define CORNER_WEIGHT 42

#include "type.h"

int getScore(char side, Board *board);
int getStabilityScore(char side, Board *board, int weight = CORNER_WEIGHT);
int getMobilityScore(char side, Board *board);
int getEdgeScore(char side, Board *board, int weight = EDGE_WEIGHT);
int getFrontierScore(char side, Board *board);

#endif
