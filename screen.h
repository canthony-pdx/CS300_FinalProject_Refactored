#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include <string>
#include "common.h"

using namespace std;
 
class screen
{
	public:

		screen();   //set up windows, pads, color pairs
		~screen();  //tear down widows, pads, color pairs

		// unless otherwise specified, functions return OK on success, other

		// item constants: FOOD, TOOL, OBSTACLE, CHEST, SHIP, BINOCULARS, HEROCHAR, NONE
		// terrain constants: MEADOW, SWAMP, WATER, WALL, HERO, DIAMOND
		// place an item and/or a terrain on the screen at x,y
		int put(int x, int y, char item, int terrain);

		// if you use something besides the declared constants above,
		// be sure to type cast with (char) or (int)
		int put(int x, int y, char item);
		int put(int x, int y, int terrain);

		// direction constants: NORTH, SOUTH, EAST, WEST
		// change the cursor location
		int moveCursor(int direction);
		int putCursor(int x, int y);

		// returns cursor x and y
		int getCursorX();
		int getCursorY();

		// clear map and menu windows
		int init();

		// get key from viewPort window (non-blocking)
		int getKey();

		// refresh map and viewPort windows
		int refreshWin();

		// center viewPort on coordinate
		int center(int x, int y);
    
		// print to top menu
		int printtomenu(int num, const char * info);
		int printtobot(int num, const char * info);

		int printtomenu(string info);
		int printtobot(string info);
		int clearmenu();
		int clearbot();

	private:

		int resize();

		WINDOW* viewPort;    //displayed viewport
		WINDOW* divider;     //displayed divider
		WINDOW* topMargin;
		WINDOW* menu;        //displayed menu
        	WINDOW* bot_menu;    // bottom portion
		WINDOW* arrows;      //map edge indicators

		int x = 0;           //cursor position
		int y = 0;           //cursor position
		int heroX = 0;       //hero position
		int heroY = 0;       //hero position

		int viewPortWidth;
		int viewPortHeight;
		int pminrow;         // upper left corner of viewPort rect
		int pmincol;         // upper left corner of viewPort rect

		int minMenuWidth = 20;
		int menuWidth;
};

#endif /* SCREEN_H */
