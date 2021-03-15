// expand to load game state from savefile
// currently used for loading level file, which is a subset of savefile

#include "loader.h"
#include "common.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>	// transform
#include <cctype>	// tolower

using namespace std;

loader::loader() {
}

loader::~loader() {
	for (auto ptr : obstacles) {
		delete ptr;
	}
	for (auto ptr : foods) {
		delete ptr;
	}
	for (auto ptr : tools) {
		delete ptr;
	}
}

int loader::loadIn(const char* filename, Array* map) {

	ifstream infile(filename);
	string line;

	const int LMAP = 1;
	const int LHERO = 2;
	const int LFOOD = 3;
	const int LTOOL = 4;
	const int LOBSTACLE = 5;
	const int LCLUE = 6;
	const int LCHEST = 7;


	int section = 0;
	int mapLine = 0;


	while (getline(infile, line)) {
		line = line.substr(0, line.find("#")); // truncate comments at #

		istringstream s(line);
		string word;

		if (!(s >> word)) {  // blank line
			continue;
		}
		// select section
		if (word.rfind("[", 0) == 0) {
		transform(word.begin(), word.end(), word.begin(), ::tolower); // lowercase the section header
		}
		if (word.rfind("[map]", 0) == 0) {
			section = LMAP;
			mapLine = 0;
			continue;
		}
		if (word.rfind("[hero]", 0) == 0) {
			section = LHERO;
			continue;
		}
		if (word.rfind("[food]", 0) == 0) {
			section = LFOOD;
			continue;
		}
		if (word.rfind("[tool]", 0) == 0) {
			section = LTOOL;
			continue;
		}
		if (word.rfind("[obstacle]", 0) == 0) {
			section = LOBSTACLE;
			continue;
		}
		if (word.rfind("[clue]", 0) == 0) {
			section = LCLUE;
			continue;
		}
		if (word.rfind("[chest]", 0) == 0) {
			section = LCHEST;
			continue;
		}

		if (section == 0) {
			continue;	// we havent declared a section yet
		}

		switch(section) {
			case LMAP :
				{
					if (word.length() == 128) {
						for (int x = 0; x < 128; ++x) {
							char c = word.at(x);
							int terrain = 0;
							switch (c) {
								case '$':
									terrain = DIAMOND;
									break;
								case 'w':
								case 'W':
									terrain = WATER;
									break;
								case 'l':
								case 'L':
									terrain = WALL;
									break;
								case 'o':
								case 'O':
									terrain = MEADOW;
									break;
								case 'x':
								case 'X':
									terrain = SWAMP;
									break;
							}
							if (terrain) {
								map->place_terrain(mapLine, x, terrain);
							}
						}
					++mapLine;
					}
				}
				break;
			case LHERO :
					if (word.compare("X") == 0 || word.compare("x") == 0) {
						s >> heroX;
					} else {
						if (word.compare("Y") == 0 || word.compare("y") == 0) {
							s >> heroY;
						}
					}
				break;
			case LFOOD :
				{
					Food* thisThing = new Food;

					string name = word;
					if (name.rfind("\"", 0) == 0) {		// name is quoted, so add to it until we find a terminating quote
						while (name.back() != '\"') {
							string nextpart;
							s >> nextpart;
							name = name + " " + nextpart;
						}
					}
					if (name.front() == '\"') {
						name.erase(0,1);
					}
					if (name.back() == '\"') {
						name.pop_back();
					}
					thisThing->name = name;

					s >> thisThing->cost >> thisThing->energy;

					int x, y;
					s >> x >> y;

					foods.push_back(thisThing);

					map->set_food(y, x, thisThing);
				}
				break;
			case LTOOL :
				{
					Tool* thisThing = new Tool;

					string name = word;
					if (name.rfind("\"", 0) == 0) {	 // name is quoted, so add to it until we find a terminating quote
						while (name.back() != '\"') {
							string nextpart;
							s >> nextpart;
							name = name + " " + nextpart;
						}
					}
					if (name.front() == '\"') {
						name.erase(0,1);
					}
					if (name.back() == '\"') {
						name.pop_back();
					}
					thisThing->name = name;

					s >> thisThing->cost >> thisThing->energyDiv >> thisThing->type;

					int x, y;
					s >> x >> y;

					tools.push_back(thisThing);

					map->set_tool(y, x, thisThing);
				}
				break;
			case LOBSTACLE :
                                {
                                        Obstacle* thisThing = new Obstacle;

                                        string name = word;
                                        if (name.rfind("\"", 0) == 0) {  // name is quoted, so add to it until we find a terminating quote
                                                while (name.back() != '\"') {
                                                        string nextpart;
                                                        s >> nextpart;
                                                        name = name + " " + nextpart;
                                                }
                                        }
                                        if (name.front() == '\"') {
                                                name.erase(0,1);
                                        }
                                        if (name.back() == '\"') {
                                                name.pop_back();
                                        }
                                        thisThing->name = name;

                                        s >> thisThing->cost >> thisThing->type;

                                        int x, y;
                                        s >> x >> y;

                                        obstacles.push_back(thisThing);

	                                map->set_obstacle(y, x, thisThing);
                                }
				break;
			case LCLUE :
				{
					bool clueBool = false;
					transform(word.begin(), word.end(), word.begin(), ::tolower); // lowercase the true/false
					if (word.rfind("true", 0) == 0) {
						clueBool = true;
					}

					int x, y;
					s >> x >> y;

					map->set_clue(y, x, clueBool);
				}
				break;
			case LCHEST :
				{
					int chaChing = 0;
					int x, y;
					s >> chaChing >> x >> y;

					map->set_chest(y, x, chaChing);
				}
					break;
			default :
				continue;

		}

	}

	infile.close();
	return 0;

}

int loader::getHeroCol() {
	return heroX;
}

int loader::getHeroLine() {
	return heroY;
}
