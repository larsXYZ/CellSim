#include "cell.h"
#include "SETTINGS.h"
#include "world.h"
#include "DNA.h"

#include <stdlib.h>
#include <cmath>

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
	
	//Some plantcells give away energy to similar cells
	if (DNA->foodtype == 0) energy_transfer();
	
	
	//Cells can do different actions. If they move we must tell the rest of the program
	int movement_indicator = 0;
	
	//Cells crawl
	movement_indicator += crawl();
	
	//Tries to eat plants if herbivore, and quits function if it did
	if (DNA->foodtype == 1)
	{
		movement_indicator += eatPlant();
	}
	
	//Tries to eat animal if carnivore
	if (DNA->foodtype == 2)
	{
		movement_indicator += eatAnimal();
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
	
	//Only eats if it is hungry
	if (energy > eatingHungerLimit) return 0;
	
	//Gets random direction
	int* dir = getRandomDirection(8); //Gives 8 as an argument so we do not get a preferred directio sim
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
	return 0;
}

int cell::eatAnimal()
{
	//Checks eatchance
	if (eatChanceAnimal > rand()%100) return 0;	
	
	//Only eats if it is hungry
	if (energy > eatingHungerLimit) return 0;
	
	//Gets random direction
	int* dir = getRandomDirection(8); //Gives 8 as an argument so we do not get a preferred directio sim
	int dx = dir[0];
	int dy = dir[1];
	delete dir;
	
	int index = world->vectorToIndex(xpos+dx,ypos+dy);
	if (index == -1) return 0; //Index out of range, something wrong
	
	if (world->grid[index].life != NULL)
	{
		cell* victim = world->grid[index].life;
		if (victim->DNA->foodtype == 1)
		{
			energy += victim->energy;
			delete victim;
			world->grid[index].life = NULL;
			world->moveToLocation(world->vectorToIndex(xpos,ypos),xpos+dx,ypos+dy);
			return 1;
		}
	}
}

void cell::energy_transfer()
{
	//It cant happen all the time, dice rolls
	if (energy < energy_transfer_limit || rand() % 100 > DNA->energy_transfer_rate) return;
	
	//Gets direction
	int* dir = getRandomDirection(8);
	int dx = dir[0];
	int dy = dir[1];
	delete dir;
	
	//Checks if chosen target is occupied
	if (world->grid[world->vectorToIndex(xpos+dx,ypos+dy)].life == NULL) return;
	cell* target = world->grid[world->vectorToIndex(xpos+dx,ypos+dy)].life;
	
	//Target must be pretty similar to giver
	int tr = target->DNA->color_red;
	int tg = target->DNA->color_green;
	int tb = target->DNA->color_blue;
	int r = DNA->color_red;
	int g = DNA->color_green;
	int b = DNA->color_blue;
	int difference = abs(tr - r) + abs(tg - g) + abs(tb - b);
	
	if (difference > energy_transfer_likeness_limit) return;
	
	//Target cell must have less energy that giver
	if (target->energy > energy) return;
	
	//Gives cell some energy
	target->energy += energy_transfer_amount;
	energy -= energy_transfer_amount;
}

void cell::duplicate()
{
	//Checks energylevels
	if (energy < breedingHungerLimit) return;

	//Checks breedchance
	if(rand()%100 > DNA->breedrate[DNA->foodtype]) return;
	
	//Chooses direction
	int *dir;
	dir = getRandomDirection(DNA->preferred_duplication_dir);
	int dx = dir[0];
	int dy = dir[1];
	delete dir;
		
	//Checks if choosen spawn spot is valid
	int index = world->vectorToIndex(xpos+dx,ypos+dy);
	if (index == -1) return;
	
	//Creates child and gives on mutated genes
	if (world->grid[index].type == AIR)
	{
		//Plants dont kill to make room for its child
		if (world->grid[index].life != NULL && DNA->foodtype == 0) return;
		
		//Other will
		if (world->grid[index].life != NULL) delete world->grid[index].life;
		
		world->grid[index].life = new cell(world);
		cell* child = world->grid[index].life;
		
		child->xpos = xpos + dx;
		child->ypos = ypos + dy;
		child->xvel = xvel;
		child->yvel = yvel;
		child->energy = breedingHungerLimit/2;
		delete child->DNA;
		child->DNA = new DNAOBJECT(DNA);
		
		energy -= breedingHungerLimit/2;
	}
}

int cell::crawl()
{
	if (!DNA->stationary)
	{
		
		//Only does this if not hungry
		if (energy < movementHungerLimit) return 0;
				
		//Changes direction
		if (rand() % 100 < DNA->turnfreq)
		{						
			int* dir = getRandomDirection(8); //8 -> no preferred direction
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

int* cell::getRandomDirection(int pref)
{
	int dir = rand() % 8;
	if (pref != 8 && rand() % 2) dir = pref; //Simulates preferred direction	
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
