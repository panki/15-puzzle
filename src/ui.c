#include <locale.h>
#include "game.h"
#include "ui.h"

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
    mvprintw(getmaxy(stdscr) - 2, (getmaxx(stdscr) - 21) / 2, "© 2019 Игра «Пятнашки»");
    mvprintw(getmaxy(stdscr) - 1, (getmaxx(stdscr) - 50) / 2, "Куросвая работа Панько Д.А. <daniil.panko@mail.ru>");
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
    menu_items[ITEM_PLAY] = "Новая игра";
    menu_items[ITEM_HELP] = "Помощь    ";
    menu_items[ITEM_EXIT] = "Выход     ";

    WINDOW *w = create_window(20, 7, "Меню");

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

    s.frame = create_window(framew, frameh, "Игра");

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

        if (g->board[row][col] > 0)
        {
            box(w, 0, 0);
            mvwprintw(w, 1, 2, "%2d", g->board[row][col]);
        }
        else
        {
            wclear(w);
        }
        wrefresh(w);
    }
    mvwprintw(s->frame, getmaxy(s->frame) - 1, (getmaxx(s->frame) - 16) / 2, " номер хода: %d ", g->moves);
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
    char height = 12; // Высота окна с рамкой

    WINDOW *frame = create_window(width, height, "Помощь");
    WINDOW *w = derwin(frame, height - 4, width - 8, 2, 4); // Новое окно с отступами для текста

    wprintw(w, "Управление:\n\n");
    wprintw(w, "← ↑ ↓ → - перемещение фишек\n");
    wprintw(w, "      N - начать игру заново\n");
    wprintw(w, "      Q - закончить игру и выйти в меню\n");
    wprintw(w, "\n\nНажмите любую клавишу для возврата в меню");
    wrefresh(w);

    wgetch(frame); // Ждем нажатия на любую клавишу

    close_window(w);
    close_window(frame);
}

void win()
{
    WINDOW *w = create_window(50, 5, "Вы выиграли");
    mvwprintw(w, 2, 4, "Нажмите любую клавишу для возврата в меню");
    wgetch(w);
    close_window(w);
}

void play()
{
    bool play = true;
    Game game = new_game();
    Screen screen = new_game_screen();

    do
    {
        update_game_screen(&screen, &game);
        int key = wgetch(screen.frame);
        switch (key)
        {
        case KEY_RIGHT:
            move_to(&game, LEFT);
            break;
        case KEY_DOWN:
            move_to(&game, UP);
            break;
        case KEY_LEFT:
            move_to(&game, RIGHT);
            break;
        case KEY_UP:
            move_to(&game, DOWN);
            break;
        case 'n':
        case 'N':
            game = new_game();
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

    clear_game_screen(&screen);
}
