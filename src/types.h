#ifndef GAME_HEADER
#define GAME_HEADER

typedef char byte;

#define BOARD_SIZE 4
#define BOARD_CELLS BOARD_SIZE *BOARD_SIZE
#define MAXRC BOARD_SIZE - 1
#define ROW(i) i / BOARD_SIZE
#define COL(i) i % BOARD_SIZE

#define SHUFFLE_COUNT 4

typedef struct game_state
{
    byte board[BOARD_SIZE][BOARD_SIZE];
    byte row;
    byte col;
    unsigned int moves;
} game_state;

#endif