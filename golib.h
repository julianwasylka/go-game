#ifndef GOLIB_H
#define GOLIB_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

//zmienne przechowujace miejsce poczatku ramki
extern int xborderstart, yborderstart;

//ulozenie planszy i legendy
typedef struct _layout {
	int x_legend_placement;
	int y_legend_placement;
	int x_board_placement;
	int y_board_placement;
} layout;

//informacje o planszy
typedef struct _board_info {
	int x;
	int y;
	unsigned char znak;
} board_info;

//punktacja
typedef struct _score {
	int player1;
	int player2;
} score;

//informacje o stanie gry
typedef struct _game_info {
	int board_size;
	int legend_placement;
	int counter;
	bool handicap;
	score score;
	board_info** board;
	unsigned char previous_board_state1[30][30];
	unsigned char previous_board_state2[30][30];
} game_info;

//zainicjalizowanie danych gry
game_info game_setup(int board_size, int legend_placement);

//wyczyszczenie danych
void destroy_game(game_info game);

//sprawdzenie, czy nie wyjdzie si� poza tablice
bool inrangex(int start, int position, game_info game);
bool inrangey(int start, int position, game_info game);

//zwracanie kamienia ulozonego o 1 dalej w danym kierunku 
char adjacent(int dir, int xstart, int xposition, int ystart, int yposition, game_info game);

//sprawdzenie, czy kamien nie bedzie samob�jczy
bool kill(char gracz, int xstart, int xposition, int ystart, int yposition, game_info game);

//rozwiazanie walki ko
void ko_fight(int x, int y, game_info* game, char player, int xstart, int xposition, int ystart, int yposition);

//ulozenie planszy oraz legendy
layout set_layout();

//narysowanie ramki oraz rozwidlen na planszy
void draw_border(int xstart, int ystart, game_info game);
void draw_board(int xstart, int ystart, game_info game, bool is_empty);

//usuwanie planszy z ramka
void clear_board_with_frame(int x, int y, int game_size);

//zapisanie zmiany w danym miejscu w tablicy
void kill_stone(int y, int x, game_info* game);
void put_stone(int x, int y, game_info* game, char player, int xstart, int xposition, int ystart, int yposition);

//zapisanie stanu planszy
void save_board_state(game_info* game);

//sprawdzenie, czy ko zostało użyte
bool used_ko(int x, int y, game_info* game, char player, int xstart, int xposition, int ystart, int yposition);

//zaladowanie ulozenia kamieni z tabeli
void loadboard(int x, int y, game_info game);

//resetowanie gry do jej pocz�tkowego stanu
void new_game(int xstart, int ystart, int *x, int *y, game_info *game);

//przesuwanie wierszy w legendzie
int legend_txt_y(int* y, int step);

//usuwanie planszy z ramka
void clear_board_with_frame(int x, int y, int game_size);

//zmiana rozmiaru planszy
int change_board_size(int xl, int yl);

//pobieranie indexu planszy z dostepnych plansz 'boards'
int get_board_index(int boards[], int boards_size, game_info game);

//zmiana rozmiaru planszy
int change_board_size(int xl, int yl);

//alokuje miejsce na plansze
board_info** init_board(int board_size);

//wczytanie i zapis gry
bool load_game(char* file_name, game_info* game);
void save_game(char* file_name, game_info game);

//wczytanie nazwy pliku
void enter_file_name(int xl, int yl, char* txt, int txt_size);

//wyczyszczenie i wyswietlenie planszy
void clear_board(int xborderstart, int yborderstart, game_info game);

//sprawdzenie czy trwa pierwsza runda
bool is_first_round(game_info* game);

#ifdef __cplusplus
}
#endif

#endif
