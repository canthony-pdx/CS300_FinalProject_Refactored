#include <iostream>
#include <cstring>
#include <cctype>
#include "common.h"

using namespace std;
struct tool{	//Tool data type
	string name;
	int type;
	int energyDiv;
};

struct node{	//Linked list of tools
	Tool data;
	node * next;
};

class player
{
	public:
		player ();
		~player ();
		 
		int getWhiffles();	//Player whiffle functions
		int loseWhiffles(int x);
		int addWhiffles(int x);

		int getEnergy();	//Player energy functions
		int loseEnergy(int x);
		int addEnergy(int x);

		node * getBino();
		int loseBino();
		int addBino(Tool * bino);

		int inShip(Tool * bino);
		node * outShip();

		int displayTools(); //Wrappers for the linked list

		int addTool(Tool * to_add);

		node * retrieve(int check);
		
		int remove(Tool & item);
	private:
		int energy;	//Private data types and functions
		int whiffles;
		int ship;
		int bino;
		node * head;
		node ** array_head;
		int array_size;
		int addTool(node * & head, Tool * to_add);	//Linked list recursive functions
		int displayTools(node * head);
		int retrieve(node * head,  int check);
		int remove (node *  & head, Tool & item);
		int removeAll(node * &head);

};
