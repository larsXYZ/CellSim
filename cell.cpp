#include "cell.h"
#include "SETTINGS.h"
#include "world.h"
#include "DNA.h"

#include <stdlib.h>

cell::cell()
{
	DNA = new DNAOBJECT();
	world = NULL;
	xpos = rand() % xsize;
	ypos = 1;
	energy = 100;
	age = 0;
}

cell::cell(worldObject* w)
{
	DNA = new DNAOBJECT();
	world = w;
	xpos = rand() % xsize;
	ypos = 10;
	energy = 100;
	age = 0;
}

void cell::live()
{
	//Ages
	age++;	
	
	//Gain energy from sunlight
	if (energy < 100) energy++;
	
	//Breed
	duplicate();
}

void cell::duplicate()
{
	//Checks energylevels
	if (energy < 60) return;

	//Checks breedchance
	if(rand()%100 > DNA->breedrate) return;
	
	//Chooses direction
	int dir = rand() % 8;	
	int dx, dy;
		
	switch (dir)
	{
	case 0:
		{	
		dx = 0;
		dy = -1;
		break;
		}	
	case 1:
		{
		dx = 1;
		dy = -1;
		break;
		}
	case 2:
		{
		dx = 1;
		dy = 0;
		break;
		}
	case 3:
		{
		dx = 1;
		dy = 1;
		break;
		}
	case 4:
		{
		dx = 0;
		dy = 1;
		break;
		}
	case 5:
		{
		dx = -1;
		dy = 1;
		break;
		}
	case 6:
		{
		dx = -1;
		dy = 0;
		break;
		}
	case 7:
		{
		dx = -1;
		dy = -1;
		break;
		}
	}	
	
	//Checks if choosen spawn spot is valid
	int index = world->vectorToIndex(xpos+dx,ypos+dy);
	if (index == -1) return;
	
	//Creates child and gives on mutated genes
	if (world->grid[index].type == AIR && world->grid[index].life == NULL)
	{
		world->grid[index].life = new cell(world);
		cell* child = world->grid[index].life;
		
		child->xpos = xpos + dx;
		child->ypos = ypos + dy;
		child->energy = 50;
		delete child->DNA;
		child->DNA = new DNAOBJECT(DNA);
		
		energy -= 50;
	}
	
	
	
	
}
