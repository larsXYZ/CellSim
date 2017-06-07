#include "cell.h"
#include "SETTINGS.h"
#include "world.h"
#include "DNA.h"

#include <stdlib.h>

cell::cell()
{
	hasMoved = false;
	DNA = new DNAOBJECT();
	world = NULL;
	xpos = rand() % xsize;
	ypos = 1;
	xvel = 0;
	yvel = 0;
	energy = 100;
	age = 0;
}

cell::cell(worldObject* w)
{
	hasMoved = false;
	DNA = new DNAOBJECT();
	world = w;
	xpos = rand() % xsize;
	ypos = 10;
	xvel = 0;
	yvel = 0;
	energy = 100;
	age = 0;
}

int cell::live()
{	
	
	//Ages
	age++;
	
	//Requires energy to live
	energy -= lifeCost;
	
	//Breed
	duplicate();
	
	//Gets energy from photosynthesis if plant
	int lightStrength = world->grid[world->vectorToIndex(xpos,ypos)].lightStrength;
	if (DNA->foodtype == 0 && lightStrength > 0) energy += photosynthesisStrength*lightStrength; 
	
	

	
	//Cells can do different actions. If they move we must tell the rest of the program
	int movement_indicator = 0;
	
	//Cells crawl
	movement_indicator += crawl();
	
	//Tries to eat plants if herbivore, and quits function if it did
	if (DNA->foodtype == 1)
	{
		if (eatPlant()) return movement_indicator;
	}
	
	//Cells fall
	if (world->isFree(xpos,ypos+1))
	{
		world->moveToLocation(world->vectorToIndex(xpos,ypos),xpos,ypos+1);
		movement_indicator += 1;
	}
	
	//Limits energy to 100
	if (energy > 100) energy = 100;
	return movement_indicator;
}


int cell::eatPlant()
{
	//Checks eatchance
	if (eatChancePlant > rand()%100) return 0;	
	
	//Gets random direction
	int* dir = getRandomDirection();
	int dx = dir[0];
	int dy = dir[1];
	delete dir;
	
	int index = world->vectorToIndex(xpos+dx,ypos+dy);
	if (index == -1) return 0; //Index out of range, something wrong
	
	if (world->grid[index].life != NULL)
	{
		cell* victim = world->grid[index].life;
		if (victim->DNA->foodtype == 0)
		{
			energy += victim->energy;
			delete victim;
			world->grid[index].life = NULL;
			world->moveToLocation(world->vectorToIndex(xpos,ypos),xpos+dx,ypos+dy);
			return 1;
		}
	}
}

void cell::duplicate()
{
	//Checks energylevels
	if (energy < 60) return;

	//Checks breedchance
	if(rand()%100 > DNA->breedrate[DNA->foodtype]) return;
	
	//Chooses direction
	int *dir;
	dir = getRandomDirection();
	int dx = dir[0];
	int dy = dir[1];
	delete dir;
		
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
		child->xvel = xvel;
		child->yvel = yvel;
		child->energy = 50;
		delete child->DNA;
		child->DNA = new DNAOBJECT(DNA);
		
		energy -= 50;
	}
}

int cell::crawl()
{
	if (!DNA->stationary)
	{
				
		//Changes direction
		if (rand() % 100 < DNA->turnfreq)
		{						
			int* dir = getRandomDirection();
			xvel = dir[0];
			yvel = dir[1];
			delete dir;
		}
		
		//Moves
		if (rand() % 100 < DNA->movefreq)
		{	
			if (world->isFree(xpos+xvel,ypos+yvel)) 
			{
				world->moveToLocation(world->vectorToIndex(xpos,ypos),xpos+xvel,ypos+yvel);
				energy -= movementCost;
				return 1;
			}
		}
	}
	
	return 0;
}

int* cell::getRandomDirection()
{
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
	
	int* result = new int[2];
	result[0] = dx;
	result[1] = dy;
	return result;
}
