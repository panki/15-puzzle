#ifndef UI_HEADER
#define UI_HEADER

#include <ncurses.h>
#include "game.h"

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
void play();
MENU_ITEM main_menu(MENU_ITEM);

#endif