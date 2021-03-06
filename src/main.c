#include <time.h>
#include <stdlib.h>
#include "ui.h"

void run()
{
    srand(time(0));
    while (true)
    {
        MENU_ITEM i = main_menu(i);
        switch (i)
        {
        case ITEM_PLAY:
            play();
            break;
        case ITEM_HELP:
            help();
            break;
        case ITEM_EXIT:
            return;
        }
    }
}

int main()
{
    init_screen();
    run();
    endwin();
    return 0;
}