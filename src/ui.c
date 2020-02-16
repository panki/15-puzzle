#include <locale.h>
#include <string.h>
#include "game.h"
#include "ui.h"
#include "solver.h"
#include "history.h"

int key2dir(int key)
{
    switch (key)
    {
    case KEY_UP:
        return UP;
    case KEY_DOWN:
        return DOWN;
    case KEY_RIGHT:
        return RIGHT;
    case KEY_LEFT:
        return LEFT;
    }
    return -1;
}

int utf8len(char *s)
{
    int len = 0;
    for (; *s; ++s)
        if ((*s & 0xC0) != 0x80)
            ++len;
    return len;
}

void init_screen()
{
    setlocale(LC_ALL, ""); // Инициализация локали (обязательна)
    initscr();             // Инициализация библиотеки ncurses
    noecho();              // Не выводить на экран при нажатии клавиш
    curs_set(FALSE);       // Не показывать курсов
    mvprintw(getmaxy(stdscr) - 2, (getmaxx(stdscr) - 23) / 2, "© 2019 Game «15-puzzle»");
    mvprintw(getmaxy(stdscr) - 1, (getmaxx(stdscr) - 39) / 2, "Authors: Panko Alexander & Panko Daniil");
    refresh();
}

WINDOW *create_window(char width, char height, char *title)
{
    char x = (getmaxx(stdscr) - width) / 2;
    char y = (getmaxy(stdscr) - height) / 2;
    WINDOW *w = newwin(height, width, y, x);
    box(w, 0, 0);
    mvwprintw(w, 0, (width - utf8len(title) - 4) / 2, "┤ %s ├", title);
    wrefresh(w);
    keypad(w, TRUE);
    return w;
}

void close_window(WINDOW *w)
{
    wclear(w);   // Очищаем окно
    wrefresh(w); // Обновляем экран
    delwin(w);   // Удаляем окно
}

MENU_ITEM main_menu(MENU_ITEM default_item)
{
    char current_item = default_item >= 0 && default_item < MENU_ITEMS_COUNT ? default_item : 0;

    char *menu_items[MENU_ITEMS_COUNT];
    menu_items[ITEM_PLAY] = "New game";
    menu_items[ITEM_HELP] = "Help    ";
    menu_items[ITEM_EXIT] = "Quit    ";

    WINDOW *w = create_window(20, 7, "Menu");

    while (true)
    {
        // печатаем иеню и текущий выбранный пункт меню
        for (char i = 0; i < MENU_ITEMS_COUNT; i++)
        {
            i == current_item ? wattron(w, A_STANDOUT) : wattroff(w, A_STANDOUT);
            mvwprintw(w, i + 2, 4, " %s ", menu_items[i]);
        }
        // ожидаем нажатия клавиши
        int key = wgetch(w);
        switch (key)
        {
        case KEY_UP:
            current_item > 0 && current_item--;
            break;
        case KEY_DOWN:
            current_item < MENU_ITEMS_COUNT - 1 && current_item++;
            break;
        case 10: // KEY_ENTER
            close_window(w);
            return current_item;
        }
    }
}

Screen new_game_screen()
{
    Screen s;

    char cw = 6; // ширина клетки
    char ch = 3; // высота клетки

    char ch_space = 0; // расстояние между клетками по вертикали
    char cw_space = 1; // расстояние между клетками по горищонтали

    char cx = 2; // отступ до клеток по горищонтали
    char cy = 1; // отступ до клеток по вертикали

    char framew = cw * BOARD_SIZE + cw_space * (BOARD_SIZE + 1) + 2; // ширина рамки
    char frameh = ch * BOARD_SIZE + ch_space * (BOARD_SIZE + 1) + 2; // высота рамки

    s.frame = create_window(framew, frameh, "Game");

    // Создаем дочерние окна для каждой клетки
    for (char i = 0; i < BOARD_CELLS; i++)
    {
        char row = ROW(i), col = COL(i);
        char x = cx + col * (cw + cw_space);
        char y = cy + row * (ch + ch_space);
        s.board[row][col] = derwin(s.frame, ch, cw, y, x);
    }
    return s;
}

void update_game_screen(Screen *s, Game *g)
{
    for (char i = 0; i < BOARD_CELLS; i++)
    {
        char row = ROW(i), col = COL(i);
        WINDOW *w = s->board[row][col];

        if (g->board[row][col] != EMPTY_CELL)
        {
            box(w, 0, 0);
            mvwprintw(w, 1, 2, "%2d", g->board[row][col] + 1);
        }
        else
        {
            wclear(w);
        }
        wrefresh(w);
    }
    mvwprintw(s->frame, getmaxy(s->frame) - 1, (getmaxx(s->frame) - 11) / 2, " Steps: %d ", g->moves);
    wrefresh(s->frame);
}

void clear_game_screen(Screen *s)
{
    for (char i = 0; i < BOARD_CELLS; i++)
    {
        char row = ROW(i), col = COL(i);
        WINDOW *w = s->board[row][col];
        close_window(w);
    }
    close_window(s->frame);
}

void help()
{
    char width = 50;  // Ширина окна с рамкой
    char height = 13; // Высота окна с рамкой

    WINDOW *frame = create_window(width, height, "Help");
    WINDOW *w = derwin(frame, height - 4, width - 8, 2, 4); // Новое окно с отступами для текста

    wprintw(w, "Controls:\n\n");
    wprintw(w, "← ↑ ↓ → - move\n");
    wprintw(w, "      N - restart game\n");
    wprintw(w, "      S - suggest next move\n");
    wprintw(w, "    U/R - undo/redo last move\n");
    wprintw(w, "      Q - quit\n");
    wprintw(w, "\n\nPress any key to return");

    wrefresh(w);

    wgetch(frame); // Ждем нажатия на любую клавишу

    close_window(w);
    close_window(frame);
}

void win()
{
    WINDOW *w = create_window(50, 5, "You win");
    mvwprintw(w, 2, 4, "Press any key to return to the menu");
    wgetch(w);
    close_window(w);
}

void play()
{
    bool play = true;
    Direction dir;

    Game game = new_game();
    Screen screen = new_game_screen();
    History *history = history_new();
    Solution *solution = NULL;

    do
    {
        update_game_screen(&screen, &game);
        int key = wgetch(screen.frame);
        switch (key)
        {
        case KEY_UP:
        case KEY_DOWN:
        case KEY_RIGHT:
        case KEY_LEFT:
            dir = OPPOSITE_DIRECTION(key2dir(key));
            if (can_move_to(&game, dir))
            {
                history_add(history, &game);
                move_to(&game, dir);
            }
            break;
        case 'n':
        case 'N':
            game = new_game();
            history_clear(history);
            break;
        case 's':
        case 'S':
            if (!solution)
                solution = solver_solve(&game);
            history_add(history, &game);
            game = solver_suggest(&game, &solution);
            break;
        case 'u':
        case 'U':
            history_undo(history, &game);
            break;
        case 'r':
        case 'R':
            history_redo(history, &game);
            break;
        case 'q':
        case 'Q':
            play = false;
            continue;
        default:
            continue;
        }

        if (check_win(&game))
        {
            update_game_screen(&screen, &game);
            win();
            play = false;
            break;
        }
    } while (play);

    history_free(&history);
    solver_free(&solution);
    clear_game_screen(&screen);
}
