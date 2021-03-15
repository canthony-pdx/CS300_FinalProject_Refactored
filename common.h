#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>
#include<iostream>
#include<cctype>
#include<cstring>
#include <ctime>
using namespace std;

// common use data structures and constant

// items
const char FOOD = 'F'; //70
const char TOOL = 'T'; //85
const char OBSTACLE = '!'; //33
const char CHEST = '$'; //36
const char CLUE = '?';
const char SHIP = 'S'; //83
const char BINOCULARS = 'B'; //66
const char NONE = ' '; //32
const char HEROCHAR = '@';

// terrains
const int MEADOW = 1;   // black on green
const int SWAMP = 2;    // black on magenta
const int WATER = 3;    // black on blue
const int WALL = 4;     // black on white
const int HERO = 5;     // yellow on red
const int DIAMOND = 6;  // white on cyan
const int HIDDEN = 7;   // normal on black

// directions
const int NORTH = 1;
const int EAST = 2;
const int SOUTH = 3;
const int WEST = 4;


struct Food {
	string name;
	int cost;
	int energy;
};

struct Tool {
	string name;
	int cost;
	int type;
	int energyDiv;
};

struct Obstacle {
	string name;
	int type;
	int cost;
};

struct Tile{
	string name;
	int type;
};

#endif /* COMMON_H */
