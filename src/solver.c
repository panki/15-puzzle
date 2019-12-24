#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "solver.h"

typedef struct GameProbe
{
    Game game;
    int depth;
    int heuristic;
    struct GameProbe *parent;
} GameProbe;

int mdistance(Game *g)
{
    int d = 0;
    for (char i = 0; i < BOARD_CELLS; i++)
    {
        char r = ROW(i), c = COL(i), v = g->board[r][c];
        d += abs(r - ROW(v)) + abs(c - COL(v));
    }
    return d;
}

bool is_games_equal(Game *g1, Game *g2)
{
    return memcmp(g1->board, g2->board, BOARD_CELLS) == 0;
}

bool is_game_solvable(Game *g)
{
    // Algorithm from
    // https://www.geeksforgeeks.org/check-instance-15-puzzle-solvable/

    char emptyRow = g->row + 1; // first row should be odd
    int parity = 0;

    for (int i = 0; i < BOARD_CELLS; i++)
    {
        char ri = ROW(i), ci = COL(i), inotempty = g->board[ri][ci] != EMPTY_CELL;
        if (inotempty)
            for (int j = i + 1; j < BOARD_CELLS; j++)
            {
                char rj = ROW(j), cj = COL(j), jnotempty = g->board[rj][cj] != EMPTY_CELL;
                if (jnotempty && g->board[ri][ci] > g->board[rj][cj])
                    parity++;
            }
    }
    return BOARD_SIZE % 2 == 0         // if even grid
               ? emptyRow % 2 == 0     // and empty cell
                     ? parity % 2 == 0 // on odd row
                     : parity % 2 != 0 // on even row
               : parity % 2 == 0;      // else odd grid
}

GameProbe *new_game_probe(Game *g, GameProbe *parent)
{
    GameProbe *gp = malloc(sizeof(GameProbe));
    gp->game = (*g); // copy
    gp->parent = parent;
    gp->depth = parent ? parent->depth + 1 : 1;
    gp->heuristic = mdistance(&gp->game) + gp->depth;
    return gp;
}

Solution *solver_solve(Game *g)
{
    if (!is_game_solvable(g))
        return NULL;

    if (check_win(g))
        return NULL;

    Queue *backlog = queue_new();
    Queue *seen = queue_new();

    bool solved = false;

    GameProbe *current = new_game_probe(g, NULL);
    queue_insert_sorted(backlog, current, current->heuristic);

    while (backlog->size < MAX_TREE_SIZE)
    {
        current = queue_pop(backlog);
        queue_insert(seen, current);

        if (check_win(&current->game))
        {
            solved = true;
            break;
        }

        for (int dir = UP; dir <= DOWN; dir++)
        {
            Game game = current->game;

            if (!move_to(&game, dir))
                continue;

            bool exists = false;
            for (Node *n = seen->head; n; n = n->next)
            {
                GameProbe *gp = n->data;
                if (is_games_equal(&gp->game, &game))
                {
                    exists = true;
                    break;
                }
            }

            if (exists)
                continue;

            GameProbe *gp = new_game_probe(&game, current);
            queue_insert_sorted(backlog, gp, gp->heuristic);
        }
    }

    if (!solved)
        return NULL;

    // now current points to winning game state

    Solution *s = malloc(sizeof(Solution));
    s->size = current->depth;
    s->games = malloc(sizeof(Game) * s->size);

    for (int i = s->size - 1; current; current = current->parent, i--)
    {
        // fill in reverse order
        // from original game to the win
        s->games[i] = current->game;
        // fix moves count
        s->games[i].moves = g->moves + i;
    }

    // Free memory

    while (backlog->size > 0)
    {
        GameProbe *gp = queue_pop(backlog);
        free(gp);
    }

    while (seen->size > 0)
    {
        GameProbe *gp = queue_pop(seen);
        free(gp);
    }

    queue_free(&backlog);
    queue_free(&seen);

    return s;
}

Game solver_suggest(Game *g, Solution **s)
{
    assert(s != NULL);
    Solution *solution = (*s);

    // Try to find next move in existing solution
    for (int i = 0; i < solution->size; i++)
    {
        if (is_games_equal(g, &solution->games[i]))
        {
            // Return next move
            return solution->games[++i];
        }
    }

    // Invalidate if not found
    solver_free(s);

    // Solve
    solution = solver_solve(g);

    // Return original game
    // if not solved
    if (!solution)
        return (*g);

    // Save solution and return next move
    (*s) = solution;
    return solution->games[1]; // 0 - original game
}

void solver_free(Solution **s)
{
    assert(s);
    if ((*s))
    {
        free((*s)->games);
        free((*s));
        (*s) = NULL;
    }
}