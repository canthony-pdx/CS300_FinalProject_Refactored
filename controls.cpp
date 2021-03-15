#include <ncurses.h>
#include "controls.h"
#include "common.h"
using namespace std;



game_logic::game_logic()
{
	string filename = "leveltemp.dat";
	l.loadIn(filename.c_str(), &map);
	cur_x = l.getHeroCol();
	cur_y = l.getHeroLine();
	next_x = cur_x;
	next_y = cur_y;
}

game_logic::~game_logic()
{
}

void game_logic::start()
{
	s.init();

	// set up for first turn
	s.putCursor(cur_x, cur_y);
	s.center(cur_x, cur_y);
	look(cur_y, cur_x);
	discover(cur_y, cur_x);
	moveMenu();
        if ((s.getCursorY() == cur_y) && (s.getCursorX() == cur_x)) {
        	actionMenu();
        }
	updateStats();

	int key = 0;
	while (key != 'q' && hero.getEnergy()>=0 && map.get_terrain(cur_y,cur_x) != DIAMOND){

		key = s.getKey();

		if (key == '+') {
			hero.addEnergy(1000000);
			updateStats();
		}
		if (key == 'r') {
        		for (int col = 0; col < 128; ++col) {
                		for (int row = 0; row < 128; ++row)
                		{
                                	s.put(row, col, (char) whats_at(col, row), (int) map.get_terrain(col, row));
                               		map.set_seen(col, row, true);
					s.refreshWin();
                		}
        		}
		}

		//move hero
		if (key == '1' || key == '2' || key == '3' || key == '4')
		{
			move(key);
			look(cur_y, cur_x);
			discover(cur_y, cur_x);
			moveMenu();
			actionMenu();
			updateStats();
		}

		// move cursor
                if (key == KEY_UP) {
                        s.moveCursor(NORTH);
			discover(s.getCursorY(), s.getCursorX());
			moveMenu();
                }
                if (key == KEY_DOWN) {
                        s.moveCursor(SOUTH);
                        discover(s.getCursorY(), s.getCursorX());
			moveMenu();
                }
                if (key == KEY_LEFT) {
                        s.moveCursor(WEST);
                        discover(s.getCursorY(), s.getCursorX());
			moveMenu();
                }
                if (key == KEY_RIGHT) {
                        s.moveCursor(EAST);
                        discover(s.getCursorY(), s.getCursorX());
			moveMenu();
                }

		// actions
		if ((s.getCursorY() == cur_y) && (s.getCursorX() == cur_x)) {
			char item = whats_at(cur_y, cur_x);

			// action by keypress
			if (key == '5' && item != NONE) {
				switch (item) {
					case FOOD:
						s.clearmenu();
						moveMenu();
						buy_food();
						updateStats();
						break;
					case SHIP:
						s.clearmenu();
						moveMenu();
						buy_ship();
						updateStats();
						break;
					case BINOCULARS:
						s.clearmenu();
						moveMenu();
						buy_binoculars();
						updateStats();
						break;
					case TOOL:
						s.clearmenu();
						moveMenu();
						buy_tool();
						updateStats();
						break;
				}
			}

			//automatic action
			if (item != NONE) {
				switch (item) {
					case CHEST:
						s.clearmenu();
						moveMenu();
						open_chest();
						updateStats();
						break;
					case OBSTACLE:
						s.clearmenu();
						moveMenu();
						remove_obstacle();
						updateStats();
						break;
					case CLUE:
						s.clearmenu();
						moveMenu();
						display_clue();
						updateStats();
						break;
				}
			}
		}
	}
	if (hero.getEnergy()<=0)
	{
		s.clearmenu();
		s.printtomenu("\n YOU HAVE RUN OUT OF ENERGY AND LOST");
		sleep(5);
	}
	if (map.get_terrain(cur_y,cur_x) == DIAMOND)
	{
		s.clearmenu();
		s.printtomenu("\n YOU HAVE FOUND THE DIAMOND AND WON");
		sleep(5);
	}

}

//moves the player and performs various actions based on the cell
void game_logic::move(int ch)
{

	// do nothing if moving off the map
        if((ch == '1' && cur_y == 0) || (ch == '3' && cur_y == 127)
        || (ch == '4' && cur_x == 0) || (ch == '2' && cur_x == 127)) {
                return;
        }

        //int next_x = cur_x;
        //int next_y = cur_y;
	next_x = cur_x;
	next_y = cur_y;

        if(ch == '1')//check north
        {
                --next_y;
        }
        else if(ch == '3')//check south
        {
                ++next_y;
        }
        else if(ch == '4')//check west
        {
                --next_x;
        }
        else if(ch == '2')//check east
        {
                ++next_x;
        }

	//checks to see what the next tile is
	int tile_check = map.get_terrain(next_y, next_x);

	// ending a voyage, moving out of ship onto land or anther ship
	if(hero.retrieve(1) && (tile_check != WATER || whats_at(next_y, next_x) == SHIP)) {

		Tool * ship = new Tool;
		ship->name = hero.retrieve(1)->data.name;
		ship->cost = hero.retrieve(1)->data.cost;
		ship->energyDiv = 0;
		ship->type = 1;

		map.set_tool(cur_y, cur_x, ship);
		hero.outShip();
	}


	// spend movement energy
	if(tile_check == WALL || (tile_check == WATER && whats_at(next_y, next_x) != SHIP && !hero.retrieve(1)))
	// moving into wall or water without being already on or moving onto ship
	{
		hero.loseEnergy(1);
		return;
	}
	else if(tile_check == SWAMP)
	{
		hero.loseEnergy(2);
	}
	else if (!(tile_check == WATER && hero.retrieve(1)))
	//not moving to water on a ship
	{
		hero.loseEnergy(1);
	}


	//moves the player to the next tile
	if(ch == '1' && cur_y > 0)//move north
	{
		--cur_y;
	}
	else if(ch == '3' && cur_y < 127)//move south
	{
		++cur_y;
	}
	else if(ch == '4' && cur_x > 0)//move west
	{
		--cur_x;
	}
	else if(ch == '2' && cur_x < 127)//move east
	{
		++cur_x;
	}


}

//buys food, adding energy and removing whiffles from the player
void game_logic::buy_food()
{
	Food * tile_food = map.get_food(cur_y, cur_x);

	if(hero.getWhiffles() < tile_food->cost)
	{
		return;
	}


	hero.addEnergy(tile_food->energy);
	hero.loseWhiffles(tile_food->cost);
	map.remove_stuff(cur_y, cur_x);

	string str = "\nDelicious and nutritious!\nGained " + to_string(tile_food->energy) + "energy!\n";
	s.printtomenu(str);

	return;
}

//buys a tool, removing whiffles and adding a tool to the player
void game_logic::buy_tool()
{
	Tool * tile_tool = map.get_tool(cur_y, cur_x);

	if(hero.getWhiffles() < tile_tool->cost)
	{
		return;
	}

	hero.addTool(tile_tool);
	hero.loseWhiffles(tile_tool->cost);
	map.remove_stuff(cur_y, cur_x);

	string str = "\nPurchased a lovely " + tile_tool->name + "!\n";
	s.printtomenu(str);

	return;
}

//removes an obstacle from the map. checks to see if the player has a matching
//tool type and if so, uses up the tool
void game_logic::remove_obstacle()
{
	//double check function
	Obstacle * tile_obstacle = map.get_obstacle(cur_y, cur_x);
	
	node * inventory = hero.retrieve(tile_obstacle->type);
	s.printtomenu("\n> Which tool?\n");
	int x = '1';
	int z = 'z';
	node * curr = inventory;
	s.clearmenu();
	discover(cur_y,cur_x);
	s.printtomenu("\n 0) to break it without a tool\n");
	while (curr != NULL)
	{
		string tempTool;
		tempTool += "\n";
		tempTool+= x;
	       	tempTool += ") Tool: " + curr->data.name + " Reduce energy cost by " + to_string(curr->data.energyDiv) + "\n"; 
		s.printtomenu(tempTool);
		curr = curr->next;
		++x;
	}
	while((z > '9' || z < '1') && z != '0')
	{
		z = s.getKey();
	}
	curr = inventory;
	if(z<x &&z >='1')
	{
		int counter = z-'1';
		while (curr != NULL && (counter>0))
		{
			curr = curr->next;
			--counter;
		}
		hero.loseEnergy(tile_obstacle->cost/curr->data.energyDiv);
		hero.remove(curr->data);

	}
	else if (z =='0')
		hero.loseEnergy(tile_obstacle->cost);
	else 
		return;
	s.clearmenu();
	moveMenu();
	map.remove_stuff(cur_y, cur_x);

	return;
}

//opens a chest, removing a chest from the map and adding whiffles to the player
void game_logic::open_chest()
{
	hero.addWhiffles(map.get_chest(cur_y, cur_x));

	string str = "\nWow! This chest contains gems worth " + to_string(map.get_chest(cur_y, cur_x)) + " whiffles!\n";
	s.printtomenu(str);

	map.remove_stuff(cur_y, cur_x);

	return;
}

void game_logic::buy_ship()
{
	Tool * tile_ship = map.get_tool(cur_y, cur_x);

	if(hero.getWhiffles() < tile_ship->cost)
	{
		return;
	}

	hero.inShip(map.get_tool(cur_y,cur_x));
	hero.loseWhiffles(tile_ship->cost);
	map.remove_stuff(cur_y, cur_x);

	s.printtomenu("\nReady to sail! Toot toot!\n");

	return;
}

void game_logic::buy_binoculars()
{
	Tool * tile_bino = map.get_tool(cur_y, cur_x);

	if(hero.getWhiffles() < tile_bino->cost)
	{
		return;
	}

	hero.addBino(map.get_tool(cur_y,cur_x));
	hero.loseWhiffles(tile_bino->cost);
	map.remove_stuff(cur_y, cur_x);
	look(cur_y, cur_x);

	s.printtomenu("\nThese spy glasses sure do the trick!\n");

	map.remove_stuff(cur_y, cur_x);

	return;
}

void game_logic::display_clue()
{

	int x = cur_x;
	int y = cur_y;
	int flag = 0;
	int dif = 256;
	srand(time(0));

		for (int i = 0; i<=(127) ; ++i)
		{
			for (int z = 0; z<=(127) ; z++)
			{
				if(map.get_terrain(i,z) == DIAMOND && (abs(cur_y - i)+abs(cur_x - z))<dif )
				{
					x = z;
					y = i;
					flag = 1;
					dif = (abs(cur_y-i)+abs(cur_x -z));
				}

			}
		}
		if (flag == 1)
		{
			string xdir;
			string ydir; 
			if (map.get_clue(cur_x,cur_y) == true)
			{
			x = x - cur_x;
			y = y - cur_y;
			}
			else 
			{
				int new_x = x;
				int new_y = y;

				while (x == new_x && y == new_y) {
					x = rand() % 128;
					y = rand() % 128;
				}

				x = x - cur_x;
				y = y - cur_y;
			}
			if (x>0)
				xdir = "East";
			else 
				xdir = "West";
			if (y<0)
				ydir = "North";
			else
				ydir = "South";
			string clue = "\nThe royal diamonds are located " + to_string(abs(y)) + " grovniks to the " + ydir + " and " + to_string(abs(x)) + " grovniks to the " + xdir;
			const char * cluechar = clue.c_str();
			s.printtomenu(cluechar);
		}

	map.remove_stuff(cur_y,cur_x);
}

void game_logic::look(int heroLine, int heroCol) {
	int range;
	if(hero.getBino())
		range = 2;
	else 
		range = 1;

        for (int c = -1 * range; c <= range; ++c) {
                for (int l = -1 * range; l <= range; ++l)
                {
			int row = l + heroLine;
			int col = c + heroCol;

			if (row >= 0 && row <= 127 && col >= 0 && col <= 127) {
				s.put(col, row, whats_at(row, col), map.get_terrain(row, col));
				map.set_seen(row, col, true);
			}
                }
	}

	s.put(heroCol, heroLine, HEROCHAR, HERO);
	s.putCursor(heroCol, heroLine);
}

char game_logic::whats_at(int row, int col) {

        if (row < 0 || row > 127 || col < 0 || col > 127) {
		return '\0';  // return null character if out of range
	}

	if (map.get_food(row, col)) {
		return FOOD;
	}

	if (map.get_tool(row, col)) {
		if (map.get_tool(row, col)->type == 1) {
			return SHIP;
		}
		if (map.get_tool(row,col)->type == 0) {
			return BINOCULARS;
		}
		return TOOL;
	}

	if (map.get_obstacle(row, col)) {
		return OBSTACLE;
	}

	if (map.get_chest(row, col)) {
		return CHEST;
	}

	if (map.is_clue(row, col)) {
		return CLUE;
	}

	return NONE;
}

void game_logic::discover(int row, int col) {

	if (!map.is_seen(row, col)) {
		s.clearmenu();
		return ;
	}

	char item = whats_at(row, col);

	string out;

	switch (item) {
		case FOOD:
			{
				Food * food = map.get_food(row, col);
				out = "> Food: " + food->name + "\n> Cost: " + to_string(food->cost) + "\n> Energy: " +to_string(food->energy) + "\n";
			}
			break;
		case SHIP:
			{
				Tool * ship = map.get_tool(row, col);
				out = "> Ship: " + ship->name + "\n> Cost: " + to_string(ship->cost) + "\n";
			}
			break;
		case BINOCULARS:
			{
				Tool * binoculars = map.get_tool(row, col);
				out = "> Binoculars: " + binoculars->name + "\n> Cost: " + to_string(binoculars->cost) + "\n";
			}
			break;
		case TOOL:
			{
				Tool * tool = map.get_tool(row, col);
				out = "> Tool: " + tool->name + "(" + to_string(tool->energyDiv) + "X)\n> Cost: " + to_string(tool->cost) + "\n";
			}
			break;
		case OBSTACLE:
			{
				Obstacle * obstacle = map.get_obstacle(row, col);
				out = "> Obstacle: " + obstacle->name + "\nEnergy required: " + to_string(obstacle->cost) + "\n";
			}
			break;
		case CHEST:
			out = "> An ornate chest.\n";
			break;
		case CLUE:
			out = "> A clue toward your destiny!\n";
	}

	if (map.get_terrain(row, col) == DIAMOND) {
		out = "> The Royal Diamond\n";
	}

	s.clearmenu();
	s.printtomenu(out);

	return;
}

void game_logic::moveMenu() {
	s.printtomenu("Options:\n1) North\n2) East\n3) South\n4) West\n");
	return;
}

void game_logic::actionMenu() {
	char item = whats_at(cur_y, cur_x);

	string out = "\n";

	switch (item) {
		case FOOD:
			out = "\n5) Purchase and eat food\n";
			break;
		case SHIP:
			out = "\n5) Purchase passage for a voyage\n";
			break;
		case BINOCULARS:
			out = "\n5) Purchase these binoculars\n";
			break;
		case TOOL:
			out = "\n5) Purchase this tool\n";
			break;
		case OBSTACLE:
			out = "\n5) Remove this obstacle\n";
			break;
	}

	s.printtomenu(out);
	return;
}

void game_logic::updateStats() {

        string ENE = "Energy: " + to_string(hero.getEnergy()) + " ";
        const char *echar = ENE.c_str();

        string WH = "Whiffles: " + to_string(hero.getWhiffles()) + " ";
        const char *wchar = WH.c_str();

        s.printtobot(1, wchar);
        s.printtobot(2, echar);

	return;
}
