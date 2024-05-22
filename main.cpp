#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include"conio2.h"
#include"config.h"
#include"golib.h"

//ok

int main() {
	int zn = 0, zero = 0;
	game_info game = game_setup(BOARD_SIZE, LEGEND_PLACEMENT);
    game_info tmp_game = { 0 };

	int boards[] = {BOARD_SIZE_9, BOARD_SIZE_13, BOARD_SIZE_19, game.board_size};
	int boards_size = sizeof(boards) / sizeof(int);
	int board_size_pick = get_board_index(boards, boards_size, game);
	int xl_custom_size = 0, yl_custom_size = 0;

	char cursor = CURSOR;

	layout layout = set_layout();
	int x = layout.x_board_placement;
	int y = layout.y_board_placement;
	int xlegend = layout.x_legend_placement;
	int ylegend = layout.y_legend_placement;

	xborderstart = x - 1;
	yborderstart = y - 1;

	//bufor sluzacy do wypisywania tekstu w legendzie
	char txt[64] = {0};

	char save_file_name[16] = { 0 };
    int save_file_name_size = sizeof(save_file_name) / sizeof(char);
    int xl_save_file_name = 0, yl_save_file_name = 0;

    char load_file_name[16] = { 0 };
    int load_file_name_size = sizeof(load_file_name) / sizeof(char);
    int xl_load_file_name = 0, yl_load_file_name = 0;

	// if the program is compiled in pure C, then we need to initialize
	// the library ourselves; __cplusplus is defined only if a C++ compiler
	// is used
#ifndef __cplusplus
	Conio2_Init();
#endif

	settitle("Go game");

	// hide the blinking cursor
	_setcursortype(_NOCURSOR);

	draw_board(xborderstart, yborderstart, game, true);

	do {
		//wyrysowanie legendy
		ylegend = layout.y_board_placement; 
		gotoxy(xlegend, ylegend);
		cputs("Go");
		gotoxy(xlegend, legend_txt_y(&ylegend, 1));
		cputs("implemented functionalities = a,b,c,d,e,f,g,h,k");
		gotoxy(xlegend, legend_txt_y(&ylegend, 2));
		cputs("q        = exit");
		gotoxy(xlegend, legend_txt_y(&ylegend, 1));
		cputs("n        = start a new game");
		gotoxy(xlegend, legend_txt_y(&ylegend, 1));
		cputs("cursors  = moving");
		gotoxy(xlegend, legend_txt_y(&ylegend, 1));
		cputs("i        = pick a stone");
		gotoxy(xlegend, legend_txt_y(&ylegend, 1));
		cputs("enter    = put a stone");
		gotoxy(xlegend, legend_txt_y(&ylegend, 1));
		cputs("escape   = cancel the move");
		gotoxy(xlegend, legend_txt_y(&ylegend, 2));
		sprintf(txt, "position = %2d, %2d", (x - xborderstart) / 2 + 1, y - yborderstart);
		cputs(txt);
		gotoxy(xlegend, legend_txt_y(&ylegend, 1));
		sprintf(txt, "round    = %2d", game.counter);
		cputs(txt);
		gotoxy(xlegend, legend_txt_y(&ylegend, 1));
		cputs("score: ");
		gotoxy(xlegend, legend_txt_y(&ylegend, 1));
		sprintf(txt, "player 1 (O) = % 2d", game.score.player1);
		cputs(txt);
		gotoxy(xlegend, legend_txt_y(&ylegend, 1));
		sprintf(txt, "player 2 (@) = % 2d", game.score.player2);
		cputs(txt);

        gotoxy(xlegend, legend_txt_y(&ylegend, 2));
		sprintf(txt, "l        = load file: %-15s", load_file_name);
		cputs(txt);
		xl_load_file_name = xlegend + 22;
		yl_load_file_name = ylegend;
		gotoxy(xlegend, legend_txt_y(&ylegend, 1));
		sprintf(txt, "s        = save to file: %-15s", save_file_name);
		cputs(txt);
		xl_save_file_name = xlegend + 25;
		yl_save_file_name = ylegend;

		//menu wyboru rozmiaru planszy (wyswietlane tylko w pierwszej rundzie)
		if (is_first_round(&game))
		{
			gotoxy(xlegend, legend_txt_y(&ylegend, 2));
        	sprintf(txt, "h        = handicap : %-3s", game.handicap ? "On" : "Off");
        	cputs(txt);
			gotoxy(xlegend, legend_txt_y(&ylegend, 2));
			cputs("b        = change board size");
			gotoxy(xlegend, legend_txt_y(&ylegend, 1));
			sprintf(txt, "%3s  : 9x9", board_size_pick == 0 ? "-->" : "   ");
			cputs(txt);
			gotoxy(xlegend, legend_txt_y(&ylegend, 1));
			sprintf(txt, "%3s  : 13x13", board_size_pick == 1 ? "-->" : "   ");
			cputs(txt);
			gotoxy(xlegend, legend_txt_y(&ylegend, 1));
			sprintf(txt, "%3s  : 19x19", board_size_pick == 2 ? "-->" : "   ");
			cputs(txt);
			gotoxy(xlegend, legend_txt_y(&ylegend, 1));
			sprintf(txt, "%3s  : custom: %d", board_size_pick == 3 ? "-->" : "   ", boards[3]);
			cputs(txt);
			xl_custom_size = xlegend + 15;
			yl_custom_size = ylegend;
		}
		else
		{
			int tmpy = ylegend;
			const char* empty_menu = "                              ";
			for (int i = 0; i < 2; i++) // ukrycie handicap i zmiany planszy
            {
            	gotoxy(xlegend, legend_txt_y(&tmpy, 2));
            	cputs(empty_menu);
            }
			for (int i = 0; i < boards_size; i++)
			{
				gotoxy(xlegend, legend_txt_y(&tmpy, 1));
				cputs(empty_menu);
			}
		}

		//wyrysowanie planszy
		draw_border(xborderstart, yborderstart, game);
		loadboard(xborderstart+2, yborderstart+1, game);
		//wyrysowanie kursora
		gotoxy(x + 1, y);
		putch(cursor);

		// we wait for key press and get its code
		// most key codes correspond to the characters, like
		// a is 'a', 2 is '2', + is '+', but some special keys
		// like cursors provide two characters, where the first
		// one is zero, e.g., "up arrow" is zero and 'H'
		zero = 0;
		zn = getch();
		// we do not want the key 'H' to play role of "up arrow"
		// so we check if the first code is zero
		if (zn == 0)
		{
			zero = 1;															// if this is the case then we read
			zn = getch();														// the next code knowing that this
			if (zn == CURSOR_UP && inrangey(yborderstart, y - 1, game)) y--;	// will be a special key
			else if (zn == CURSOR_DOWN && inrangey(yborderstart, y + 1, game)) y++;
			else if (zn == CURSOR_LEFT && inrangex(xborderstart, x - 2, game)) x -= 2;
			else if (zn == CURSOR_RIGHT && inrangex(xborderstart, x + 2, game)) x += 2;
		}
		else if (zn == 'i' && !game.handicap &&
			game.board[y - yborderstart - 1][(x - xborderstart - 1) / 2].znak != SYMBOL_1 && //podnoszenie kamienia, o ile
			game.board[y - yborderstart - 1][(x - xborderstart - 1) / 2].znak != SYMBOL_2)   //kursor nie jest juz nad innym
		{
			if (game.counter % 2 == 0) cursor = SYMBOL_1;
			else cursor = SYMBOL_2;
		}
		else if (zn == ENTER																	//stawianie kamienia jezeli
			&& cursor != CURSOR																	//w danym polu juz zaden nie lezy, oraz
			&& game.board[y - yborderstart - 1][(x - xborderstart - 1) / 2].znak != SYMBOL_1	//nie jest on samobojczy oraz
			&& game.board[y - yborderstart - 1][(x - xborderstart - 1) / 2].znak != SYMBOL_2	//nie przeczy to zasadzie ko
			&& used_ko((x - xborderstart - 1) / 2, y - yborderstart - 1, &game, cursor, xborderstart, x, yborderstart, y) == false)
		{
			if (is_first_round(&game)==false) ko_fight((x - xborderstart - 1) / 2, y - yborderstart - 1, &game, cursor, xborderstart, x, yborderstart, y);
			if (kill(cursor, xborderstart, x, yborderstart, y, game) == false)
			{
				put_stone((x - xborderstart - 1) / 2, y - yborderstart - 1, &game, cursor, xborderstart, x, yborderstart, y);
				if (!game.handicap)
				    cursor = CURSOR;

				if (is_first_round(&game) == false) save_board_state(&game);
			}
		}
		else if (zn == ESCAPE) //anulowanie ruchu
		{
		    game.handicap = false;
		    cursor = CURSOR;
		}
		else if (zn == 'n')	//reset gry
		{
			new_game(xborderstart + 1, yborderstart + 1, &x, &y, &game);
			if(!game.handicap)
			    cursor = CURSOR;
		}
		else if (zn == 'b' && is_first_round(&game)) //wybor rozmiaru planszy
		{
			++board_size_pick %= boards_size;

			int tmpsize = boards[boards_size - 1];
			struct text_info info = { 0 };
			gettextinfo(&info);

			if (board_size_pick == 3) 
			{
				tmpsize = change_board_size(xl_custom_size, yl_custom_size);
				if (tmpsize < (info.screenwidth - SET_LEGEND_RIGHT)/2)
				{
					boards[3] = tmpsize;

                    clear_board(xborderstart, yborderstart, game);
					game = game_setup(boards[board_size_pick], LEGEND_PLACEMENT);
					x = xborderstart + 1;
					y = yborderstart + 1;
					draw_board(xborderstart, yborderstart, game, true);
				}
			}
			clear_board(xborderstart, yborderstart, game);
			game = game_setup(boards[board_size_pick], LEGEND_PLACEMENT);
			x = xborderstart + 1;
			y = yborderstart + 1;
			draw_board(xborderstart, yborderstart, game, true);
		}
		else if (zn == 's') //zapis gry do pliku
        {
        	enter_file_name(xl_save_file_name, yl_save_file_name, save_file_name, save_file_name_size);
        	save_game(save_file_name, game);
        }
        else if (zn == 'l') //odczyt gry z pliku
        {
        	enter_file_name(xl_load_file_name, yl_load_file_name, load_file_name, load_file_name_size);
        	if (load_game(load_file_name, &tmp_game))
        	{
        		clear_board(xborderstart, yborderstart, game);
        		game = tmp_game;

        		x = xborderstart + 1;
        		y = yborderstart + 1;
        		draw_board(xborderstart, yborderstart, game, false);
        	}
        }
        else if (zn == 'h' && is_first_round(&game)) //zainicjowanie handicapa
        {
        	game.handicap = !game.handicap;
        	if (game.handicap)
        		cursor = SYMBOL_1;
        	else
        		cursor = CURSOR;
        }
		
	} while (zn != 'q'); //zakonczenie programu

	destroy_game(game);

	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be
									// visible after the program ends
	return 0;
}