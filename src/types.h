#ifndef GAME_HEADER
#define GAME_HEADER

typedef char byte;

#define BOARD_SIZE 4
#define SHUFFLE_COUNT 4

typedef struct game_state
{
    byte board[BOARD_SIZE][BOARD_SIZE];
    byte row;
    byte col;
    unsigned int moves;
} game_state;

#endif