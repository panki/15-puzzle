#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "game.h"

bool move_to(Game *g, Direction dir)
{
    char row = g->row, col = g->col;

    switch (dir)
    {
    case UP:
        row--;
        break;
    case DOWN:
        row++;
        break;
    case LEFT:
        col--;
        break;
    case RIGHT:
        col++;
        break;
    default:
        return false;
    }

    if (col < 0 || col > MAXRC || row < 0 || row > MAXRC)
        return false;

    char value = g->board[row][col];
    if (value == TARGET_VALUE(row, col))
    {
        g->wrong_cells++;
    }
    else if (value == TARGET_VALUE(g->row, g->col))
    {
        g->wrong_cells--;
    }
    g->board[g->row][g->col] = g->board[row][col];
    g->board[row][col] = EMPTY_CELL;
    g->moves++;
    g->row = row;
    g->col = col;
    return true;
}

void shuffle_board(Game *g)
{
    // previous direction
    int prev = -1;

    for (int i = 0; i < SHUFFLE_COUNT;)
    {
        int dir = RANDOM_DIRECTION;
        if (dir != OPPOSITE_DIRECTION(prev) && move_to(g, dir))
        {
            prev = dir;
            i++;
        }
    }

    // clear after shuffle
    g->moves = 0;
}

Game new_game()
{
    Game g;
    for (char i = 0; i < BOARD_CELLS; i++)
    {
        char row = ROW(i), col = COL(i);
        g.board[row][col] = i;
    }

    g.row = g.col = MAXRC;
    g.wrong_cells = 0;
    g.moves = 0;

    shuffle_board(&g);
    return g;
}

bool check_win(Game *g)
{
    return g->wrong_cells == 0;
}