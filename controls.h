#include "player.h"
#include "screen.h"
#include "loader.h"

class game_logic
{

	public:
		game_logic();
		~game_logic();
		void move(int ch);
		void buy_food();
		void buy_tool();
		void remove_obstacle();
		void open_chest();
		void buy_ship();
		void buy_binoculars();
		void display_clue();
		void start();

	private:
		player hero;
		Array map;
		screen s;
		loader l;
		int cur_x;
		int cur_y;
		int next_y;
		int next_y;

		void look(int heroLine, int heroCol);
		char whats_at(int row, int col);
		void discover(int row, int col);
		void moveMenu();
		void actionMenu();
		void updateStats();
};


