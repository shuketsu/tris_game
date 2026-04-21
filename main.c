#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SQUARE 3
#define OFFSET_X 4
#define OFFSET_Y 2

#define COLOR_USER 1
#define COLOR_BOT 2
#define COLOR_CURSOR 3
#define COLOR_BOARD 4

void init_colors() {
    start_color();
    init_pair(COLOR_USER, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_BOT, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_CURSOR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BOARD, COLOR_WHITE, COLOR_BLACK);
}

void draw_board() {
    attron(COLOR_PAIR(COLOR_BOARD));
    for(int i = 0; i <= SQUARE * OFFSET_Y; i += OFFSET_Y) {
        mvhline(i, 0, '-', SQUARE * OFFSET_X);
    }
    for(int j = 0; j <= SQUARE * OFFSET_X; j += OFFSET_X) {
        mvvline(0, j, '|', SQUARE * OFFSET_Y);
    }
    for(int i = 0; i <= SQUARE * OFFSET_Y; i += OFFSET_Y) {
        for(int j = 0; j <= SQUARE * OFFSET_X; j += OFFSET_X) {
            mvaddch(i, j, '+');
        }
    }
    attroff(COLOR_PAIR(COLOR_BOARD));
}

int game_control(int Mx[SQUARE][SQUARE]) {
    for(int i = 0; i < SQUARE; i++) {
        if (Mx[i][0] != 0 && Mx[i][0] == Mx[i][1] && Mx[i][1] == Mx[i][2]) return Mx[i][0];
        if (Mx[0][i] != 0 && Mx[0][i] == Mx[1][i] && Mx[1][i] == Mx[2][i]) return Mx[0][i];
    }
    if (Mx[0][0] != 0 && Mx[0][0] == Mx[1][1] && Mx[1][1] == Mx[2][2]) return Mx[0][0];
    if (Mx[0][2] != 0 && Mx[0][2] == Mx[1][1] && Mx[1][1] == Mx[2][0]) return Mx[0][2];
    return 0;
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    init_colors();
    srand(time(NULL));

    int Mx[SQUARE][SQUARE] = {0};
    int x = 0, y = 0;
    int ch;
    bool game_running = true;
    int empty_cells = SQUARE * SQUARE;

    draw_board();

    while(game_running) {
        for(int i = 0; i < SQUARE; i++) {
            for(int j = 0; j < SQUARE; j++) {
                int py = i * OFFSET_Y + (OFFSET_Y / 2);
                int px = j * OFFSET_X + (OFFSET_X / 2);
                
                if (Mx[i][j] == 1) {
                    attron(COLOR_PAIR(COLOR_USER) | A_BOLD);
                    mvaddch(py, px, 'X');
                    attroff(COLOR_PAIR(COLOR_USER) | A_BOLD);
                } else if (Mx[i][j] == 2) {
                    attron(COLOR_PAIR(COLOR_BOT) | A_BOLD);
                    mvaddch(py, px, 'O');
                    attroff(COLOR_PAIR(COLOR_BOT) | A_BOLD);
                } else {
                    mvaddch(py, px, ' ');
                }

                if (i == y && j == x) {
                    attron(COLOR_PAIR(COLOR_CURSOR) | A_REVERSE);
                    mvaddch(py, px, (Mx[i][j] == 0) ? '?' : (Mx[i][j] == 1 ? 'X' : 'O'));
                    attroff(COLOR_PAIR(COLOR_CURSOR) | A_REVERSE);
                }
            }
        }

        int winner = game_control(Mx);
        if (winner != 0 || empty_cells == 0) {
            move(SQUARE * OFFSET_Y + 1, 0);
            if (winner == 1) printw("--- COMPLIMENTI! HAI VINTO! ---");
            else if (winner == 2) printw("--- PECCATO! HAI PERSO! ---");
            else printw("--- PAREGGIO! ---");
            refresh();
            getch();
            break;
        }

        ch = getch();
        switch(ch) {
            case KEY_UP:    if (y > 0) y--; break;
            case KEY_DOWN:  if (y < SQUARE - 1) y++; break;
            case KEY_LEFT:  if (x > 0) x--; break;
            case KEY_RIGHT: if (x < SQUARE - 1) x++; break;
            case ' ': 
                if (Mx[y][x] == 0) {
                    Mx[y][x] = 1;
                    empty_cells--;
                    
                    if (empty_cells > 0 && game_control(Mx) == 0) {
                        int bx, by;
                        do {
                            bx = rand() % SQUARE;
                            by = rand() % SQUARE;
                        } while(Mx[by][bx] != 0);
                        Mx[by][bx] = 2;
                        empty_cells--;
                    }
                }
                break;
            case 'q': game_running = false; break;
        }
        refresh();
    }

    endwin();
    return 0;
}
