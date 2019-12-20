#include <locale.h>
#include "game.h"
#include "ui.h"

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

MENU_ITEM main_menu(MENU_ITEM default_item)
{
    WINDOW *w;

    char current_item = default_item >= 0 && default_item < MENU_ITEMS_COUNT ? default_item : 0;

    char *menu_items[MENU_ITEMS_COUNT];
    menu_items[ITEM_PLAY] = "Новая игра";
    menu_items[ITEM_HELP] = "Помощь    ";
    menu_items[ITEM_EXIT] = "Выход     ";

    char width = 20;                              // Ширина окна
    char height = 7;                              // Высота окна
    char x = (getmaxx(stdscr) - width) / 2;       // По центру
    char y = (getmaxy(stdscr) - height) / 2;      // По центру
    w = newwin(height, width, y, x);              // Новое окно
    box(w, 0, 0);                                 // Рамка
    mvwprintw(w, 0, (width - 8) / 2, "┤ Меню ├"); // Заголовок
    wrefresh(w);                                  // отобразить окно
    keypad(w, TRUE);                              // разрешить ввод с клавиатуры для окна

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
        case 10:         // KEY_ENTER
            wclear(w);   // Очищаем окно
            wrefresh(w); // Обновляем экран
            delwin(w);   // Удаляем окно
            return current_item;
        }
    }
}

screen_state new_game_screen()
{
    screen_state ss;

    char cw = 6; // ширина клетки
    char ch = 3; // высота клетки

    char ch_space = 0; // расстояние между клетками по вертикали
    char cw_space = 1; // расстояние между клетками по горищонтали

    char framew = cw * BOARD_SIZE + cw_space * (BOARD_SIZE + 1) + 2; // ширина рамки
    char frameh = ch * BOARD_SIZE + ch_space * (BOARD_SIZE + 1) + 2; // высота рамки

    char fx = (getmaxx(stdscr) - framew) / 2; // по центру
    char fy = (getmaxy(stdscr) - frameh) / 2; // по центру

    char cx = 2; // отступ до клеток по горищонтали
    char cy = 1; // отступ до клеток по вертикали

    ss.frame = newwin(frameh, framew, fy, fx); // Окно с рамкой
    box(ss.frame, 0, 0);                       // Рамка
    keypad(ss.frame, TRUE);                    // Разрешаем ввод с клавиатуры

    // Создаем дочерние окна для каждой клетки
    for (char i = 0; i < BOARD_CELLS; i++)
    {
        char row = ROW(i), col = COL(i);
        char x = cx + col * (cw + cw_space);
        char y = cy + row * (ch + ch_space);
        ss.board[row][col] = derwin(ss.frame, ch, cw, y, x);
    }
    return ss;
}

void update_game_screen(screen_state *ss, game_state *gs)
{
    box(ss->frame, 0, 0);

    for (char i = 0; i < BOARD_CELLS; i++)
    {
        char row = ROW(i), col = COL(i);

        WINDOW *win = ss->board[row][col];

        if (gs->board[row][col] > 0)
        {
            box(win, 0, 0);
            mvwprintw(win, 1, 2, "%2d", gs->board[row][col]);
        }
        else
        {
            wclear(win);
        }
    }
    mvwprintw(ss->frame, getmaxy(ss->frame) - 1, (getmaxx(ss->frame) - 16) / 2, " номер хода: %d ", gs->moves);
    wrefresh(ss->frame);
}

void clear_game_screen(screen_state *ss)
{
    for (char i = 0; i < BOARD_CELLS; i++)
    {
        char row = ROW(i), col = COL(i);
        WINDOW *win = ss->board[row][col];
        delwin(win);
    }
    wclear(ss->frame);   // Очищаем окно
    wrefresh(ss->frame); // Обновляем экран
    delwin(ss->frame);   // Удаляем окно
}

void help()
{
    WINDOW *frame, *w;

    char width = 50;                         // Ширина окна с рамкой
    char height = 12;                        // Высота окна с рамкой
    char x = (getmaxx(stdscr) - width) / 2;  // По центру
    char y = (getmaxy(stdscr) - height) / 2; // По центру

    frame = newwin(height, width, y, x);                 // Новое окно c рамкой
    box(frame, 0, 0);                                    // Рамка у окна
    mvwprintw(frame, 0, (width - 10) / 2, "┤ Помощь ├"); // Заголовок
    wrefresh(frame);                                     // Отобразить окно

    w = derwin(frame, height - 4, width - 8, 2, 4); // Новое окно с отступами для текста

    wprintw(w, "Управление:\n\n");
    wprintw(w, "← ↑ ↓ → - перемещение фишек\n");
    wprintw(w, "      N - начать игру заново\n");
    wprintw(w, "      Q - закончить игру и выйти в меню\n");
    wprintw(w, "\n\nНажмите любую клавишу для возврата в меню");

    wrefresh(w);

    wgetch(frame); // Ждем нажатия на любую клавишу

    delwin(w);       // Удаляем окно с текстом
    wclear(frame);   // Очищаем окно с рамкой
    wrefresh(frame); // Обновляем экран
    delwin(frame);   // Удаляем окно
}

void win()
{
    WINDOW *frame;

    char width = 50;                         // Ширина окна с рамкой
    char height = 5;                         // Высота окна с рамкой
    char x = (getmaxx(stdscr) - width) / 2;  // По центру
    char y = (getmaxy(stdscr) - height) / 2; // По центру

    frame = newwin(height, width, y, x);                    // Новое окно c рамкой
    box(frame, 0, 0);                                       // Рамка у окна
    mvwprintw(frame, 0, (width - 13) / 2, " Вы выиграли "); // Заголовок
    wrefresh(frame);                                        // Отобразить окно

    mvwprintw(frame, 2, 4, "Нажмите любую клавишу для возврата в меню");

    wgetch(frame); // Ждем нажатия на любую клавишу

    wclear(frame);   // Очищаем окно с рамкой
    wrefresh(frame); // Обновляем экран
    delwin(frame);   // Удаляем окно
}

void play()
{
    bool play = true;
    game_state gs = new_game();
    screen_state ss = new_game_screen();

    do
    {
        update_game_screen(&ss, &gs);
        int key = wgetch(ss.frame);
        switch (key)
        {
        case KEY_RIGHT:
            move_left(&gs);
            break;
        case KEY_DOWN:
            move_up(&gs);
            break;
        case KEY_LEFT:
            move_right(&gs);
            break;
        case KEY_UP:
            move_down(&gs);
            break;
        case 'n':
        case 'N':
            gs = new_game();
            break;
        case 'q':
        case 'Q':
            play = false;
            continue;
        default:
            continue;
        }
        if (check_win(&gs))
        {
            update_game_screen(&ss, &gs);
            win();
            play = false;
            break;
        }
    } while (play);

    clear_game_screen(&ss);
}
