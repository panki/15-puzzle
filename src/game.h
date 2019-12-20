#ifndef GAME_HEADER
#define GAME_HEADER

#include <stdbool.h>

#define BOARD_SIZE 4
#define BOARD_CELLS BOARD_SIZE *BOARD_SIZE
#define MAXRC BOARD_SIZE - 1
#define ROW(i) i / BOARD_SIZE
#define COL(i) i % BOARD_SIZE

#define SHUFFLE_COUNT 4

typedef struct game_state
{
    char board[BOARD_SIZE][BOARD_SIZE];
    char row;
    char col;
    unsigned int moves;
} game_state;

game_state new_game();
bool check_win(game_state *);
bool move_up(game_state *);
bool move_down(game_state *);
bool move_left(game_state *);
bool move_right(game_state *);

#endif