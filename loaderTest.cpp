#include <string>
#include "loader.h"

using namespace std;

int main() {
	Array* map = new Array;

	string filename = "savefile.dat";
	loader l;

	l.loadIn(filename.c_str(), map);

	return 0;
}
