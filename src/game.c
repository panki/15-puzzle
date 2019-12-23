#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "game.h"

bool move_up(Game *gs)
{
    if (gs->row == 0)
        return false;
    gs->board[gs->row][gs->col] = gs->board[gs->row - 1][gs->col];
    gs->board[gs->row - 1][gs->col] = 0;
    gs->moves++;
    gs->row--;
    return true;
}

bool move_down(Game *g)
{
    if (g->row == MAXRC)
        return false;
    g->board[g->row][g->col] = g->board[g->row + 1][g->col];
    g->board[g->row + 1][g->col] = 0;
    g->moves++;
    g->row++;
    return true;
}

bool move_left(Game *g)
{
    if (g->col == 0)
        return false;
    g->board[g->row][g->col] = g->board[g->row][g->col - 1];
    g->board[g->row][g->col - 1] = 0;
    g->moves++;
    g->col--;
    return true;
}

bool move_right(Game *g)
{
    if (g->col == MAXRC)
        return false;
    g->board[g->row][g->col] = g->board[g->row][g->col + 1];
    g->board[g->row][g->col + 1] = 0;
    g->moves++;
    g->col++;
    return true;
}

void shuffle_board(Game *g)
{
    time_t t;
    srand((unsigned)time(&t));
    for (int i = 0; i < SHUFFLE_COUNT;)
    {
        int d = RANDOM_DIRECTION;
        switch (d)
        {
        case UP:
            move_up(g) && i++;
            break;
        case LEFT:
            move_left(g) && i++;
            break;
        case RIGHT:
            move_right(g) && i++;
            break;
        case DOWN:
            move_down(g) && i++;
            break;
        }
    }
    g->moves = 0; // clear after shuffle
}

Game new_game()
{
    Game g;
    for (char i = 0; i < BOARD_CELLS; i++)
    {
        char row = ROW(i), col = COL(i);
        g.board[row][col] = i + 1;
    }

    g.row = g.col = MAXRC;
    g.board[g.row][g.col] = 0;

    shuffle_board(&g);
    return g;
}

bool check_win(Game *g)
{
    if (g->col != MAXRC && g->row != MAXRC)
        return false;

    for (char i = 0; i < BOARD_CELLS - 1; i++)
    {
        char row = ROW(i), col = COL(i);
        if (g->board[row][col] != i + 1)
            return false;
    }
    return true;
}