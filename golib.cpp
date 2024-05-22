#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "golib.h"
#include "conio2.h"
#include "config.h"

#ifdef __cplusplus
#define EXTERNC	extern "C"
#else
#define EXTERNC
#endif

int xborderstart, yborderstart;

game_info game_setup(int board_size, int legend_placement)
{
	board_info** board = init_board(board_size);
	score score = { 0, 0 };
	game_info game = { board_size, legend_placement, 1, false, score, board };

	return game;
}

void destroy_game(game_info game)
{
	for (int i = 0; i < game.board_size; i++)
		free(game.board[i]);
	free(game.board);
}

bool inrangex(int start, int position, game_info game)
{
	int positiononboard = (position - start + 1)/2;
	if (positiononboard <= game.board_size && positiononboard > 0)
		return true;
	return false;
}

bool inrangey(int start, int position, game_info game)
{
	int positiononboard = position - start;
	if (positiononboard <= game.board_size && positiononboard > 0)
		return true;
	return false;
}

char adjacent(int dir, int xstart, int xposition, int ystart, int yposition, game_info game)
{
	int x = ((xposition - xstart + 1) / 2);
	int y = yposition - ystart;
	x--;
	y--;

	if (dir == DOWN)
	{
		if (inrangey(ystart, yposition + 1, game) == false)
			return BORDER;
		else return game.board[y + 1][x].znak;
	}
	else if (dir == UP)
	{
		if (inrangey(ystart, yposition - 1, game) == false)
			return BORDER;
		else return game.board[y - 1][x].znak;
	}
	else if (dir == RIGHT)
	{
		if (inrangex(xstart, xposition + 2, game) == false)
			return BORDER;
		else return game.board[y][x + 1].znak;
	}
	else if (dir == LEFT)
	{
		if (inrangex(xstart, xposition - 2, game) == false)
			return BORDER;
		else return game.board[y][x - 1].znak;
	}
	return EMPTY;
}


bool kill(char gracz, int xstart, int xposition, int ystart, int yposition, game_info game)
{
	char opponent = SYMBOL_1;
	if (gracz == SYMBOL_1) opponent = SYMBOL_2;

	int enemies = 0;

	if (adjacent(UP, xstart, xposition, ystart, yposition, game) == BORDER
		|| adjacent(UP, xstart, xposition, ystart, yposition, game) == opponent)
		enemies++;
	if (adjacent(DOWN, xstart, xposition, ystart, yposition, game) == BORDER
		|| adjacent(DOWN, xstart, xposition, ystart, yposition, game) == opponent)
		enemies++;
	if (adjacent(LEFT, xstart, xposition, ystart, yposition, game) == BORDER
		|| adjacent(LEFT, xstart, xposition, ystart, yposition, game) == opponent)
		enemies++;
	if (adjacent(RIGHT, xstart, xposition, ystart, yposition, game) == BORDER
		|| adjacent(RIGHT, xstart, xposition, ystart, yposition, game) == opponent)
		enemies++;

	return (enemies == 4);
}

layout set_layout()
{
	layout position;

	if (LEGEND_PLACEMENT == LEGEND_LEFT)
	{
		position.x_legend_placement = SET_LEGEND_LEFT;
		position.x_board_placement = SET_LEGEND_RIGHT;
	}
	else
	{
		position.x_legend_placement = SET_LEGEND_RIGHT;
		position.x_board_placement = SET_LEGEND_LEFT;
	}

	position.y_legend_placement = HEIGHT;
	position.y_board_placement = HEIGHT;

	return position;
}

void draw_border(int xstart, int ystart, game_info game)
{
	int i;
	
	gotoxy(xstart, ystart);
	putch(TOP_LEFT_CORNER);

	gotoxy(xstart + 2 * (game.board_size)+2, ystart);
	putch(TOP_RIGHT_CORNER);
	
	for (i = 1; i < 2*(game.board_size)+2; i++)
	{
		gotoxy(xstart + i, ystart);
		putch(BORDER_X);
	}
	
	for (i = 1; i < game.board_size+1; i++)
	{
		gotoxy(xstart, ystart + i);
		putch(BORDER_Y);

		gotoxy(xstart + 2*(game.board_size + 1), ystart + i);
		putch(BORDER_Y);
	}

	gotoxy(xstart, ystart + game.board_size + 1);
	putch(BOTTOM_LEFT_CORNER);

	gotoxy(xstart + 2 * (game.board_size)+2, ystart + game.board_size + 1);
	putch(BOTTOM_RIGHT_CORNER);

	for (i = 1; i < 2*(game.board_size)+2; i++)
	{
		gotoxy(xstart + i, ystart + game.board_size + 1);
		putch(BORDER_X);
	}
}

void draw_board(int xstart, int ystart, game_info game, bool is_empty)
{
	xstart++;
	ystart++;

	for (int i = 0; i < game.board_size; i++)
	{
		for (int j = 0; j < game.board_size; j++)
		{
			if (i != 0 && i != game.board_size)
			{
				gotoxy(xstart + i * 2, ystart + j);
				putch(BORDER_X);
			}

            if (is_empty)
			{
			    if (i == 0)
			    {
				    if (j == 0)
				    	game.board[j][i].znak = TOP_LEFT_CORNER;
				    else if (j == game.board_size - 1)
				    	game.board[j][i].znak = BOTTOM_LEFT_CORNER;
				    else
				    	game.board[j][i].znak = BORDER_YX_LEFT;
			    }
			    else if (i == game.board_size - 1)
			    {
				    if (j == 0)
				    	game.board[j][i].znak = TOP_RIGHT_CORNER;
				    else if (j == game.board_size - 1)
				    	game.board[j][i].znak = BOTTOM_RIGHT_CORNER;
				    else
				    	game.board[j][i].znak = BORDER_YX_RIGHT;
			    }
			    else if (j == 0)
			    	game.board[j][i].znak = BORDER_XY_TOP;
			    else if (j == game.board_size - 1)
			    	game.board[j][i].znak = BORDER_XY_BOTTOM;
			    else
			    	game.board[j][i].znak = BORDER_XY;
			}
		}
	}
}

void clear_board_with_frame(int x, int y, int game_size)
{
	for (int i = 0; i < game_size + 2; i++)
	{
		gotoxy(x, y + i);
		clreol();
	}
}

void kill_stone(int y, int x, game_info* game)
{
	if (game->board[y][x].znak == SYMBOL_1)
		game->score.player1++;
	else
		game->score.player2++;

	if (x == 0)
	{
		if (y == 0)
			game->board[y][x].znak = TOP_LEFT_CORNER;
		else if (y == game->board_size - 1)
			game->board[y][x].znak = BOTTOM_LEFT_CORNER;
		else
			game->board[y][x].znak = BORDER_YX_LEFT;
	}
	else if (x == game->board_size - 1)
	{
		if (y == 0)
			game->board[y][x].znak = TOP_RIGHT_CORNER;
		else if (y == game->board_size - 1)
			game->board[y][x].znak = BOTTOM_RIGHT_CORNER;
		else
			game->board[y][x].znak = BORDER_YX_RIGHT;
	}
	else if (y == 0)
		game->board[y][x].znak = BORDER_XY_TOP;
	else if (y == game->board_size - 1)
		game->board[y][x].znak = BORDER_XY_BOTTOM;
	else
		game->board[y][x].znak = BORDER_XY;
}

void put_stone(int x, int y, game_info *game, char player, int xstart, int xposition, int ystart, int yposition)
{
	game->board[y][x].znak = player;
	game->board[y][x].x = x + 1;
	game->board[y][x].y = y + 1;
	if(!game->handicap)
	    game->counter++;

	char opponent = SYMBOL_1;
	if (player == SYMBOL_1) opponent = SYMBOL_2;

	if (adjacent(UP, xstart, xposition, ystart, yposition, *game) == opponent)
		if (kill(opponent, xstart, xposition, ystart, yposition - 1, *game))
		{
			kill_stone(y - 1, x, game);
		}
	if (adjacent(DOWN, xstart, xposition, ystart, yposition, *game) == opponent)
		if (kill(opponent, xstart, xposition, ystart, yposition + 1, *game))
		{
			kill_stone(y + 1, x, game);
		}
	if (adjacent(LEFT, xstart, xposition, ystart, yposition, *game) == opponent)
		if (kill(opponent, xstart, xposition - 2, ystart, yposition, *game))
		{
			kill_stone(y, x - 1, game);
		}
	if (adjacent(RIGHT, xstart, xposition, ystart, yposition, *game) == opponent)
		if (kill(opponent, xstart, xposition + 2, ystart, yposition, *game))
		{
			kill_stone(y, x + 1, game);
		}
}

void ko_fight(int x, int y, game_info* game, char player, int xstart, int xposition, int ystart, int yposition)
{
	_score temp = game->score;
	put_stone(x, y, game, player, xstart, xposition, ystart, yposition);
	game->counter--;
	kill_stone(y, x, game);
	if (player == SYMBOL_1)
		game->score.player1--;
	else game->score.player2--;
}

bool used_ko(int x, int y, game_info* game, char player, int xstart, int xposition, int ystart, int yposition)
{
	if (is_first_round(game) == false)
	{
		unsigned char temp_board[30][30] = { 0 };
		for (int i = 0; i < game->board_size; i++)
		{
			for (int j = 0; j < game->board_size; j++)
			{
				temp_board[j][i] = game->board[j][i].znak;
			}
		}

		_score temp = game->score;
		put_stone(x, y, game, player, xstart, xposition, ystart, yposition);
		game->counter--;
		game->score = temp;
		int cn = game->counter - 1;

		if (cn % 2 == 0)
		{
			for (int i = 0; i < game->board_size; i++)
			{
				for (int j = 0; j < game->board_size; j++)
				{
					if (game->previous_board_state1[j][i] != game->board[j][i].znak)
					{
						for (int k = 0; k < game->board_size; k++)
						{
							for (int l = 0; l < game->board_size; l++)
							{
								game->board[l][k].znak = temp_board[l][k];
							}
						}

						return false;
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < game->board_size; i++)
			{
				for (int j = 0; j < game->board_size; j++)
				{
					if (game->previous_board_state2[j][i] != game->board[j][i].znak)
					{
						for (int k = 0; k < game->board_size; k++)
						{
							for (int l = 0; l < game->board_size; l++)
							{
								game->board[l][k].znak = temp_board[l][k];
							}
						}

						return false;
					}
				}
			}
		}

		for (int k = 0; k < game->board_size; k++)
		{
			for (int l = 0; l < game->board_size; l++)
			{
				game->board[l][k].znak = temp_board[l][k];
			}
		}
		return true;
	}
	return false;
}

void save_board_state(game_info* game)
{
	if (game->counter % 2 == 0)
	{
		for (int i = 0; i < game->board_size; i++)
		{
			for (int j = 0; j < game->board_size; j++)
			{
				game->previous_board_state1[j][i] = game->board[j][i].znak;
			}
		}
	}
	else
	{
		for (int i = 0; i < game->board_size; i++)
		{
			for (int j = 0; j < game->board_size; j++)
			{
				game->previous_board_state2[j][i] = game->board[j][i].znak;
			}
		}
	}
}

void loadboard(int x, int y, game_info game)
{
	for (int i = 0; i < game.board_size; i++)
	{
		for (int j = 0; j < game.board_size; j++)
		{
			gotoxy(i*2 + x, j + y);
			putch(game.board[j][i].znak);
		}
	}
}

void new_game(int xstart, int ystart, int *x, int *y, game_info *game)
{
	for (int i = 0; i < game->board_size; i++)
	{
		for (int j = 0; j < game->board_size; j++)
		{
			game->board[j][i].znak = ' ';
		}
	}
	game->counter = 1;
	game->score.player1 = 0;
	game->score.player2 = 0;

	clrscr();
	*x = xstart;
	*y = ystart;
	gotoxy(xstart, ystart);
	draw_board(xstart-1, ystart-1, *game, true);
}

int legend_txt_y(int* y, int step)
{
	return *y += step;
}

int get_board_index(int boards[], int boards_size, game_info game)
{
	size_t index = 0;
	while (index < boards_size && boards[index] != game.board_size) ++index;
	return index % boards_size;
}

int change_board_size(int xl, int yl)
{
	int c1, c2;

	gotoxy(xl, yl);
	_setcursortype(_NORMALCURSOR);
	do
	{
		c1 = getch();
	} while (c1 < '1' || c1 > '9');
	gotoxy(xl + 1, yl);

	do
	{
		c2 = getch();
	} while ((c2 < '0' || c2 > '9') && c2 != ENTER);

	_setcursortype(_NOCURSOR);
	if (c2 == ENTER) return c1 - 48;
	
	// char '1' to int 49
	return (c1 - 48) * 10 + (c2 - 48);
}

void enter_file_name(int xl, int yl, char* txt, int txt_size) 
{
	int c2 = ' ';
	int i = 0;

	memset(txt, 0, txt_size);
	for (int i = 0; i < txt_size-1; i++)
	{
		txt[i] = c2;
	}

	gotoxy(xl, yl);
	cputs(txt);

	gotoxy(xl, yl);
	memset(txt, 0, txt_size);
	_setcursortype(_NORMALCURSOR);

	do
	{
		c2 = getche();
		if (c2 != ENTER && (c2 >= 'a' && c2 <= 'z'))
			txt[i++] = c2;
	} while ((c2 >= 'a' && c2 <= 'z') && c2 != ENTER && (i > 0 && i < 15));

	_setcursortype(_NOCURSOR);
}

board_info** init_board(int board_size)
{
	board_info** board = (board_info**)calloc(board_size, sizeof(board_info*));
	for (int i = 0; i < board_size; i++)
	{
		board[i] = (board_info*)calloc(board_size, sizeof(board_info));
	}
	return board;
}

void save_game(char* file_name, game_info game) 
{
	FILE* fp = fopen(file_name, "w");
	if (fp)
	{
		fwrite(&game, sizeof(game_info), 1, fp);
		for (int i = 0; i < game.board_size; i++) 
		{
			for (int j = 0; j < game.board_size; j++) 
			{
				fwrite(&game.board[j][i], sizeof(board_info), 1, fp);
			}
		}
		fclose(fp);
	}
}

bool load_game(char* file_name, game_info* game) 
{
	FILE* fp = fopen(file_name, "r");
	if (fp)
	{
		fread(game, sizeof(game_info), 1, fp);
		game->board = init_board(game->board_size);
		for (int i = 0; i < game->board_size; i++) 
		{
			for (int j = 0; j < game->board_size; j++) 
			{
				fread(&game->board[j][i], sizeof(board_info), 1, fp);
			}
		}
		fclose(fp);
		return true;
	}
	return false;
}

void clear_board(int xborderstart, int yborderstart, game_info game)
{
	clear_board_with_frame(xborderstart, yborderstart, game.board_size);
	destroy_game(game);
}

bool is_first_round(game_info *game)
{
	return game->counter == 1;
}