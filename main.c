#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define SQUARE 3
#define X 5
#define Y X

void gm_space(int x, int y){

	char ch1 = '|';
	char ch2 = '-';
	for(int i = 0; i < y; i++){
		for(int j = 0; j < x; j++){
			if (j % 2 != 0 && i % 2 == 0) {
				mvaddch(i, j, ch1);
			}
			if(i % 2 != 0){
				mvaddch(i, j, ch2);
			}
		}
		printw("\n");
	}
	move(0, 0);
}

int game_control(int size, int Mx[size][size]){

        for(int i = 0; i < size; i++){
                if (Mx[i][0] != 0 && Mx[i][0] == Mx[i][1] && Mx[i][1] == Mx[i][2]) {
                        return Mx[i][0];
                }
                if (Mx[0][i] != 0 && Mx[0][i] == Mx[1][i] && Mx[1][i] == Mx[2][i]) {
                        return Mx[0][i];
                }
        }

        if (Mx[0][0] != 0 && Mx[0][0] == Mx[1][1] && Mx[1][1] == Mx[2][2]) {
                return Mx[0][0];
        }

        if (Mx[0][2] != 0 && Mx[0][2] == Mx[1][1] && Mx[1][1] == Mx[2][0]) {
                return Mx[0][2];
        }

        return 0;
}

int main(){

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	int ch;
	int x = 0;
	int y = 0;
	int bot_y;
	int bot_x;
	int bot_steps = (SQUARE * SQUARE) / 2;
	bool game_trigger = true;
	char user_move = 'X';
	char bot_move = 'O';
	char user = '?';
	char clean = ' ';
	move(x, y);
	srand(time(NULL));

	int Mx[SQUARE][SQUARE] = {0};

	gm_space(X, Y);

	do {
		ch = getch();
		if (Mx[y][x] == 0) mvaddch(y * 2, x * 2, clean);
		else {
			if (Mx[y][x] == 1) mvaddch(y * 2, x * 2, user_move);
			else mvaddch(y * 2, x * 2, bot_move);
		}

		refresh();
		switch (ch){
		case KEY_UP:
			if(y - 1 >= 0 && Mx[y - 1][x] == 0){
				y--;
				mvaddch(y * 2, x * 2, user);
			} else if(y - 1 >= 0){
				for(int i = 0; i < SQUARE; i++){
					if(Mx[y - 1][i] == 0){
						y--;
						x = i;
						mvaddch(y * 2, x * 2, user);
						break;
					}
				}
			} else if (y == SQUARE - 1) {
				for(int i = 0; i < SQUARE; i++){
					if(Mx[0][i] == 0){
						y = 0;
						x = i;
						mvaddch(y * 2, x * 2, user);
						break;
					}
				}
			}
			break;
		case KEY_DOWN:
			if(y + 1 < SQUARE && Mx[y + 1][x] == 0){
				y++;
				mvaddch(y * 2, x * 2, user);
			} else if (y + 1 < SQUARE){
				for(int i = 0; i < SQUARE; i++){
					if(Mx[y + 1][i] == 0){
						y++;
						x = i;
						mvaddch(y * 2, x * 2, user);
						break;
					}
				}
			} else if (y == 0){
				for(int i = 0; i < SQUARE; i++){
					if(Mx[SQUARE - 1][i] == 0){
						y = SQUARE - 1;
						x = i;
						mvaddch(y * 2, x * 2, user);
						break;
					}
				}
			}
			break;
		case KEY_LEFT:
			if (x - 1 >= 0 && Mx[y][x - 1] == 0){
				x--;
				mvaddch(y * 2, x * 2, user);
			} else if(x - 1 >= 0){
				for(int i = 0; i < SQUARE; i++){
					if(Mx[i][x - 1] == 0){
						x--;
						y = i;
						mvaddch(y * 2, x * 2, user);
						break;
					}
				}
			} else if(x == SQUARE - 1){
				for(int i = 0; i < SQUARE; i++){
					if(Mx[i][0] == 0){
						x = 0;
						y = i;
						mvaddch(y * 2, x * 2, user);
						break;
					}
				}
			}
			break;
		case KEY_RIGHT:
			if(x + 1 < SQUARE && Mx[y][x + 1] == 0){
				x++;
				mvaddch(y * 2, x * 2, user);
			} else if (x + 1 < SQUARE){
				for(int i = 0; i < SQUARE; i++){
					if(Mx[i][x + 1] == 0){
						x++;
						y = i;
						mvaddch(y * 2, x * 2, user);
						break;
					}
				}
			} else if (x == 0){
				for(int i = 0; i < SQUARE; i++){
					if(Mx[i][SQUARE - 1] == 0){
						x = SQUARE - 1;
						y = i;
						mvaddch(y * 2, x * 2, user);
						break;
					}
				}
			}
			break;
		case ' ':
			if (Mx[y][x] == 0) {
				mvaddch(y * 2, x * 2, user_move);
				Mx[y][x] = 1;
				if(bot_steps != 0){
					do {
						bot_x = rand() % SQUARE;
						bot_y = rand() % SQUARE;
					} while(Mx[bot_y][bot_x] != 0);
					Mx[bot_y][bot_x] = 2;
					mvaddch(bot_y * 2, bot_x * 2, bot_move);
					bot_steps--;
				}
			}
			break;
		}

		if (Mx[y][x] == 0) mvaddch(y * 2, x * 2, user);
		if (game_control(SQUARE, Mx) != 0){
			if(game_control(SQUARE, Mx) == 1){
				move(0, X + 1);
				printw("you win");
				game_trigger = false;
			} else {
				move (0, X + 1);
				printw("you lose");
				game_trigger = false;
			}
		}
	} while(game_trigger == true);

	getch();
	endwin();
}

