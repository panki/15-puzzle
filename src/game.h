#ifndef GAME_HEADER
#define GAME_HEADER

#include <stdbool.h>

#define BOARD_SIZE 4
#define BOARD_CELLS BOARD_SIZE *BOARD_SIZE
#define MAXRC BOARD_SIZE - 1
#define ROW(i) i / BOARD_SIZE
#define COL(i) i % BOARD_SIZE

#define SHUFFLE_COUNT 4

typedef enum DIRECTION
{
    // Order is matter to calculate opposite
    // direction as simple math operation
    UP = 0,
    LEFT = 1,
    RIGHT = 2,
    DOWN = 3
} DIRECTION;

#define OPPOSITE_DIRECTION(d) (DOWN - (d))
#define RANDOM_DIRECTION (rand() % (DOWN + 1))

typedef struct Game
{
    char board[BOARD_SIZE][BOARD_SIZE];
    char row;
    char col;
    unsigned int moves;
} Game;

Game new_game();
bool check_win(Game *);
bool move_up(Game *);
bool move_down(Game *);
bool move_left(Game *);
bool move_right(Game *);

#endif