#ifndef __NEGASCOUT_H__
#define __NEGASCOUT_H__

#include "type.h"

MoveScore negascout(Board *board, int depth, int maxDepth, int alpha, int beta, char side);

#endif
