#ifndef GAME_HEADER
#define GAME_HEADER

#include <stdbool.h>

#define BOARD_SIZE 4
#define BOARD_CELLS BOARD_SIZE *BOARD_SIZE
#define MAXRC (BOARD_SIZE - 1)
#define ROW(i) (i / BOARD_SIZE)
#define COL(i) (i % BOARD_SIZE)
#define TARGET_VALUE(row, col) ((row)*BOARD_SIZE + (col) + 1)

#define SHUFFLE_COUNT 4

typedef struct Game
{
    char board[BOARD_SIZE][BOARD_SIZE];
    char row;
    char col;
    unsigned int moves;
    unsigned int correct_cells;
} Game;

Game new_game();
bool check_win(Game *);
bool move_up(Game *);
bool move_down(Game *);
bool move_left(Game *);
bool move_right(Game *);

#endif