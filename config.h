#ifndef CONFIG_H
#define CONFIG_H
#include "conio2.h"

//Dostêpne rozmiary planszy
#define BOARD_SIZE_9 9
#define BOARD_SIZE_13 13
#define BOARD_SIZE_19 19

//U³o¿enie gry
#define LEGEND_LEFT 0
#define LEGEND_RIGHT 1

//Pozycja legendy
#define SET_LEGEND_LEFT 10
#define SET_LEGEND_RIGHT 70
#define HEIGHT 4

//Konfiguracja gry
#define BOARD_SIZE BOARD_SIZE_9
#define LEGEND_PLACEMENT LEGEND_RIGHT

//Kierunki
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

//Znaczniki
#define CURSOR '*'
#define SYMBOL_1 '@'
#define SYMBOL_2 'O'
#define BORDER '#'
#define EMPTY '0'

//Obs³ugiwane klawisze
#define ESCAPE 0x1B
#define ENTER 0x0d
#define CURSOR_RIGHT 0x4d
#define CURSOR_LEFT 0x4b
#define CURSOR_UP 0x48
#define CURSOR_DOWN 0x50

//Wygl¹d planszy
#define TOP_LEFT_CORNER 218
#define TOP_RIGHT_CORNER 191
#define BOTTOM_LEFT_CORNER 192
#define BOTTOM_RIGHT_CORNER 217
#define BORDER_Y 179
#define BORDER_YX_LEFT 195
#define BORDER_YX_RIGHT 180
#define BORDER_X 196
#define BORDER_XY_TOP 194
#define BORDER_XY_BOTTOM 193
#define BORDER_XY 197


#endif