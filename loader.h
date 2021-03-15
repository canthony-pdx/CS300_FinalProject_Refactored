#ifndef LOADER_H
#define LOADER_H

#include "common.h"
#include "array.h"
#include <string>
#include <vector>

using namespace std;

class loader
{
	public:
		loader();
		~loader();
		int loadIn(const char* filename, Array* map);

		int getHeroCol();
		int getHeroLine();

	private:

	        vector<Food *> foods;
        	vector<Tool *> tools;
        	vector<Obstacle *> obstacles;

		int heroX = 0;
		int heroY = 0;
};

#endif /* LOADER_H */
