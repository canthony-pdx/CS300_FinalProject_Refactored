/*
 * This is the header file for the map data structure for the world
 * storing the data of each grovnick
 */
#include"common.h"

struct Cell
{
	Food * food = NULL;
	Obstacle * obstacle = NULL;
	Tool * tool = NULL;

	bool clue = false;
	bool has_clue = false;
	bool seen = false;
	int chest = 0;	
	int terrain = HIDDEN;
};

//This class stores the map data of the game.
class Array
{
	public:
	Array();
	~Array();

	//getters
	Food * get_food(int row, int col);
	Obstacle * get_obstacle(int row, int col);
	Tool * get_tool(int row, int col);
	int get_chest(int row, int col);
	bool get_clue(int row, int col); 
	bool is_clue(int row, int col);
	bool is_seen(int row, int col);
	int get_terrain(int row, int col);

	//setters
	void set_seen(int row, int col, bool revealed);
	void set_food(int row, int col, Food *);
	void set_obstacle(int row, int col, Obstacle *);
	void set_tool(int row, int col, Tool *);
	void place_terrain(int row, int col, int type);	
	void set_chest(int row, int col, int money);
	void set_clue(int row, int col, bool clue);

	bool remove_stuff(int row, int col);

	private:
	Cell ** array;	
};
