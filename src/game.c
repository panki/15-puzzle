#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "game.h"

void move_to(Game *g, char row, char col)
{
    char value = g->board[row][col];
    if (value == TARGET_VALUE(row, col))
    {
        g->correct_cells--;
    }
    else if (value == TARGET_VALUE(g->row, g->col))
    {
        g->correct_cells++;
    }
    g->board[g->row][g->col] = g->board[row][col];
    g->board[row][col] = 0;
    g->moves++;
}

bool move_up(Game *g)
{
    if (g->row == 0)
        return false;
    move_to(g, g->row - 1, g->col);
    g->row--;
    return true;
}

bool move_down(Game *g)
{
    if (g->row == MAXRC)
        return false;
    move_to(g, g->row + 1, g->col);
    g->row++;
    return true;
}

bool move_left(Game *g)
{
    if (g->col == 0)
        return false;
    move_to(g, g->row, g->col - 1);
    char value = g->board[g->row][g->col - 1];
    g->col--;
    return true;
}

bool move_right(Game *g)
{
    if (g->col == MAXRC)
        return false;
    move_to(g, g->row, g->col + 1);
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
    g.correct_cells = 15;
    g.moves = 0;

    shuffle_board(&g);
    return g;
}

bool check_win(Game *g)
{
    return g->correct_cells == BOARD_CELLS - 1;
}