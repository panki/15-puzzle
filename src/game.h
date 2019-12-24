#ifndef GAME_HEADER
#define GAME_HEADER

#include <stdbool.h>

#define BOARD_SIZE 4
#define BOARD_CELLS BOARD_SIZE *BOARD_SIZE
#define MAXRC (BOARD_SIZE - 1)
#define ROW(i) (i / BOARD_SIZE)
#define COL(i) (i % BOARD_SIZE)
#define EMPTY_CELL (BOARD_CELLS - 1)
#define TARGET_VALUE(row, col) ((row)*BOARD_SIZE + (col))

#define SHUFFLE_COUNT 4

typedef enum Direction
{
    // Order is matter to calculate opposite
    // direction as simple math operation
    UP = 0,
    LEFT = 1,
    RIGHT = 2,
    DOWN = 3
} Direction;

#define OPPOSITE_DIRECTION(d) (DOWN - (d))
#define RANDOM_DIRECTION (rand() % (DOWN + 1))

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
bool move_to(Game *, Direction);

#endif