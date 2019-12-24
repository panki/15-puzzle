#ifndef SOLVER_HEADER
#define SOLVER_HEADER

#include <game.h>
#include <pq.h>

#define MAX_TREE_SIZE 100000

typedef struct Solution
{
    Game *games;
    int size;
} Solution;

Game solver_suggest(Game *g, Solution **s);
Solution *solver_solve(Game *g);
void solver_free(Solution **s);

#endif