#ifndef UI_HEADER
#define UI_HEADER

#include <ncurses.h>
#include "types.h"

typedef struct screen_state
{
    WINDOW *frame;
    WINDOW *board[BOARD_SIZE][BOARD_SIZE];
} screen_state;

typedef enum MENU_ITEM
{
    ITEM_PLAY = 0,
    ITEM_HELP = 1,
    ITEM_EXIT = 2
} MENU_ITEM;

#define MENU_ITEMS_COUNT ITEM_EXIT - ITEM_PLAY + 1

void init_screen();
void help();
void win();
MENU_ITEM main_menu(MENU_ITEM);
screen_state new_game_screen();
void update_game_screen(screen_state *, game_state *);
void clear_game_screen(screen_state *);

#endif