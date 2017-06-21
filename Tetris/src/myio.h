#include <stdio.h>
#ifndef MYIO_H_
#define MYIO_H_

#define COLOR_DEFAULT 0
#define COLOR_BLACK 40	
#define COLOR_RED 41	//Z
#define COLOR_GREEN 42	//S
#define COLOR_BROWN 43	//O
#define COLOR_BLUE 44	//J
#define COLOR_MAGENTA 45//T
#define COLOR_CYAN 46	//I
#define COLOR_GRAY 47	//L

extern void gotoxy(int x, int y);
extern void printColorString(int x, int y, int color, char * str);
extern void printColorString(int x, int y, int color, const char * str);


#endif /* MYIO_H_ */
