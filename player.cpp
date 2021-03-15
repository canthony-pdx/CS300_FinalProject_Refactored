#include "player.h"


player::player()
{
	whiffles = 1000;
	energy = 100;
	head = NULL;
	ship = 0;
	bino = 0;
	array_size = 100;
	array_head = new node *[array_size];
	for (int i = 0; i<array_size; i++)
	{
		array_head[i]=NULL;
	}
}

node * player::getBino()
{
	return retrieve(0);
}
int player::addBino(Tool * bino)
{
	addTool((array_head[0]),bino);
	return 1;
}
int player::loseBino()
{
	array_head[0] = NULL;
	return 1;
}

int player::inShip(Tool * ship)
{
	addTool((array_head[1]),ship);
	return 1;
}
node * player::outShip()
{
	node * temp = array_head[1];
	array_head[1] = NULL;
	return temp;
}

int player::getWhiffles()	//Function handling players whiffles
{
	return whiffles;
}
int player::loseWhiffles(int x)
{
	whiffles = whiffles - x;
	return whiffles;
}
int player::addWhiffles(int x)
{
	whiffles = whiffles + x;
	return whiffles;
}

//Functions that handler the player's energy
int player::getEnergy()
{
	return energy;
}

int player::loseEnergy(int x)
{
	energy = energy - x;
	return energy;
}
int player::addEnergy(int x)
{
	energy = energy + x;
	return energy;
}

int player::displayTools()
{
	return displayTools(head);
}

int player::displayTools(node * head)
{
	for (int i=0;i<array_size;i++)
	{
		node * curr = array_head[i];
		if (curr)
			cout<<"Chain #"<<i<<endl;
		while (curr)
		{
			cout<<curr->data.name;
			curr = curr ->next;
		}

	}
	return 0;
}
int player::addTool(Tool * to_add)
{
	return	addTool((array_head[to_add->type]),to_add);
}
int player::addTool(node * & head,Tool * to_add)
{
	if(!head)
	{
		node * temp = new node;
		temp->data.name = to_add->name;
		temp->data.type = to_add->type;
		temp->data.energyDiv = to_add->energyDiv;
		temp->data.cost = to_add->cost;
		temp->next=NULL;
		head = temp;
		return 1;
	}
	return	addTool(head->next,to_add);
}

node * player::retrieve(int check)
{
	return (array_head[check]);
}
int player::retrieve(node * head,int check)
{
	if(!head)
		return 0;
	if(head->data.type == check)
	{
		cout<<head->data.name<<endl;
		return head->data.energyDiv + retrieve(head->next,check);
	}
	return 1;
}

int player::remove(Tool & item)
{
	return remove(array_head[item.type],item);
}
int player::remove(node * &head,Tool & item)
{
	if (!head)
		return 0;
	if (head->data.name.compare(item.name)==0 && head->data.energyDiv == item.energyDiv)
	{
		node * temp = head -> next;
		delete (head);
		head = temp;
		return 1;
	}
	return remove(head->next,item);
}

//Function to deallocate the list of tools
int player::removeAll(node *&head)
{
	if (head == NULL)
		return 0;
	removeAll(head->next);
	delete (head);
	return 0;
}
player::~player()
{

}

