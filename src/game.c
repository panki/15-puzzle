#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "game.h"

bool move_up(game_state *gs)
{
    if (gs->row == 0)
        return false;
    gs->board[gs->row][gs->col] = gs->board[gs->row - 1][gs->col];
    gs->board[gs->row - 1][gs->col] = 0;
    gs->moves++;
    gs->row--;
    return true;
}

bool move_down(game_state *gs)
{
    if (gs->row == MAXRC)
        return false;
    gs->board[gs->row][gs->col] = gs->board[gs->row + 1][gs->col];
    gs->board[gs->row + 1][gs->col] = 0;
    gs->moves++;
    gs->row++;
    return true;
}

bool move_left(game_state *gs)
{
    if (gs->col == 0)
        return false;
    gs->board[gs->row][gs->col] = gs->board[gs->row][gs->col - 1];
    gs->board[gs->row][gs->col - 1] = 0;
    gs->moves++;
    gs->col--;
    return true;
}

bool move_right(game_state *gs)
{
    if (gs->col == MAXRC)
        return false;
    gs->board[gs->row][gs->col] = gs->board[gs->row][gs->col + 1];
    gs->board[gs->row][gs->col + 1] = 0;
    gs->moves++;
    gs->col++;
    return true;
}

void shuffle_board(game_state *gs)
{
    time_t t;
    srand((unsigned)time(&t));
    for (int i = 0; i < SHUFFLE_COUNT;)
    {
        int d = rand() % 4;
        switch (d)
        {
        case 0:
            move_left(gs) && i++;
            break;
        case 1:
            move_up(gs) && i++;
            break;
        case 2:
            move_right(gs) && i++;
            break;
        case 3:
            move_down(gs) && i++;
            break;
        }
    }
    gs->moves = 0; // clear after shuffle
}

game_state new_game()
{
    game_state gs;
    for (byte i = 0; i < BOARD_CELLS; i++)
    {
        byte row = ROW(i), col = COL(i);
        gs.board[row][col] = i + 1;
    }

    gs.row = gs.col = MAXRC;
    gs.board[gs.row][gs.col] = 0;

    shuffle_board(&gs);
    return gs;
}

bool check_win(game_state *gs)
{
    if (gs->col != MAXRC && gs->row != MAXRC)
        return false;

    for (int i = 0; i < BOARD_CELLS - 1; i++)
    {
        byte row = ROW(i), col = COL(i);
        if (gs->board[row][col] != i + 1)
            return false;
    }
    return true;
}