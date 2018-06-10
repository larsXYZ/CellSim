#include "cell.h"
#include "SETTINGS.h"
#include "world.h"
#include "DNA.h"

#include <stdlib.h>
#include <cmath>
#include <iostream>

cell::cell()
{
	hasMoved = false;
	DNA = new DNAOBJECT();
	world = NULL;
	xpos = 0;
	ypos = 0;
	xvel = 0;
	yvel = 0;
	energy = cell_max_energy;
	age = 0;
	mergedCellParentPosX = INVALID_COORDINATE;
	mergedCellParentPosY = INVALID_COORDINATE;
	mergedCellChildPosX = INVALID_COORDINATE;
	mergedCellChildPosY = INVALID_COORDINATE;
}

cell::cell(worldObject* w)
{
	hasMoved = false;
	DNA = new DNAOBJECT();
	world = w;
	xpos = 0;
	ypos = 0;
	xvel = 0;
	yvel = 0;
	energy = cell_max_energy;
	age = 0;
	mergedCellParentPosX = INVALID_COORDINATE;
	mergedCellParentPosY = INVALID_COORDINATE;
	mergedCellChildPosX = INVALID_COORDINATE;
	mergedCellChildPosY = INVALID_COORDINATE;
}

int cell::live()
{

	//Debugging check
	if (world == NULL){
		std::cout << "World NULL" << std::endl;
		exit(0);
	}
	if (DNA == NULL){
		std::cout << "DNA NULL" << std::endl;
		exit(0);
	}

	//Ages
	age++;

	//Requires energy to live
	energy -= lifeCost;

	//Breed
	duplicate();

	photosynthesis();

	absorbSoilEnergy();

	//Some plantCells try to merge with other cells
	if (DNA->foodtype == ISPLANT && !isMerged()){
		tryToMergeChild();
		tryToMergeParent();
	}

	//Some plantCells shares energy
	if (DNA->foodtype == ISPLANT && isMerged()) energy_transfer();

	//Cells can do different actions. If they move we must tell the rest of the program
	int movement_indicator = 0;

	//Cells crawl
	movement_indicator += crawl();

	//Tries to eat plants if herbivore, and quits function if it did
	if (DNA->foodtype == HERBAVORE && !isMerged())
	{
		movement_indicator += eatPlant();
	}

	//Tries to eat animal if carnivore
	if (DNA->foodtype == CARNIVORE && !isMerged())
	{
		movement_indicator += eatAnimal();
	}

	//Limits energy to 100
	if (energy > cell_max_energy) energy = cell_max_energy;
	return movement_indicator;

}

bool cell::isMerged(){
	return (hasChild() || hasParent());
}

bool cell::hasChild(){
	return !(mergedCellChildPosX == INVALID_COORDINATE || mergedCellChildPosY == INVALID_COORDINATE);
}
bool cell::hasParent(){
	return !(mergedCellParentPosX == INVALID_COORDINATE || mergedCellParentPosY == INVALID_COORDINATE);

}

void cell::photosynthesis(){

	//Gets energy from photosynthesis if plant
	int lightStrength = world->grid[world->vectorToIndex(xpos,ypos)].lightStrength;

	//Needs air as well
	int dx;
	int dy;
	getRandomDirection(8, dx, dy); //Gives 8 as an argument so we do not get a preferred directio sim
	if (!world->isFreeAir(xpos+dx,ypos+dy)) return;

	if (DNA->foodtype == 0 && lightStrength > 0) energy += photosynthesisStrength*lightStrength;
}

void cell::absorbSoilEnergy(){

	//Gets random direction
	int dx;
	int dy;
	getRandomDirection(8, dx, dy); //Gives 8 as an argument so we do not get a preferred directio sim

	//Cannot absorb soil energy if the spot is taken
	if (!world->isFreeDirt(xpos+dx,ypos+dy)) return;

	//Some cells gets energy from the soil
	if (DNA->foodtype == 0 && world->grid[world->vectorToIndex(xpos+dx,ypos+dy)].type == DIRT && DNA->digger){
		energy += world->grid[world->vectorToIndex(xpos+dx,ypos+dy)].nutrients;
	}
}


int cell::eatPlant()
{
	//Checks eatchance
	if (eatChancePlant > rand()%100) return 0;

	//Only eats if it is hungry
	if (energy > eatingHungerLimit) return 0;

	//Gets random direction
	int dx;
	int dy;
  getRandomDirection(8, dx, dy); //Gives 8 as an argument so we do not get a preferred directio sim


	int index = world->vectorToIndex(xpos+dx,ypos+dy);
	if (index == INVALID_INDEX) return 0; //Index out of range, something wrong

	//Target must be in the AIR
	if (world->grid[index].type != AIR) return 0;

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

void cell::notifyParent(cell* target){
	target->mergedCellChildPosX = xpos;
	target->mergedCellChildPosY = ypos;
}

void cell::notifyChild(cell* target){
	target->mergedCellParentPosX = xpos;
	target->mergedCellParentPosY = ypos;
}

void cell::tryToMergeChild(){

	//Evaluates merge_rate
	if (DNA->merge_rate < rand()%100 && DNA->stationary){
	 return;
 	}

	//Gets random direction
	int dx;
	int dy;
  getRandomDirection(8, dx, dy); //Gives 8 as an argument so we do not get a preferred directio sim


	int index = world->vectorToIndex(xpos+dx,ypos+dy);
	if (index == INVALID_INDEX) return; //Index out of range, something wrong

	cell* target = world->grid[index].life;
	if (target == NULL) return;

	if (cellSimilarityTest(target))
	{

		if (target->DNA->foodtype == ISPLANT && !target->hasParent())
		{
			std::cout << "MERGING WITH CHILD" << std::endl;

			mergedCellChildPosX = target->xpos;
			mergedCellChildPosY = target->ypos;
			notifyChild(target);
		}
	}
}

void cell::tryToMergeParent(){

	//Evaluates merge_rate
	if (DNA->merge_rate < rand()%100 && DNA->stationary){
	 return;
 	}

	//Gets random direction
	int dx;
	int dy;
  getRandomDirection(8, dx, dy); //Gives 8 as an argument so we do not get a preferred directio sim


	int index = world->vectorToIndex(xpos+dx,ypos+dy);
	if (index == INVALID_INDEX) return; //Index out of range, something wrong

	cell* target = world->grid[index].life;
	if (target == NULL) return;

	if (cellSimilarityTest(target))
	{

		if (target->DNA->foodtype == ISPLANT && !target->hasChild())
		{
			std::cout << "MERGING WITH PARENT" << std::endl;

			mergedCellParentPosX = target->xpos;
			mergedCellParentPosY = target->ypos;
			notifyParent(target);
		}
	}
}

int cell::eatAnimal()
{
	//Checks eatchance
	if (eatChanceAnimal > rand()%100) return 0;

	//Only eats if it is hungry
	if (energy > eatingHungerLimit) return 0;

	//Gets random direction
	int dx;
	int dy;
	getRandomDirection(8, dx, dy); //Gives 8 as an argument so we do not get a preferred directio sim

	int index = world->vectorToIndex(xpos+dx,ypos+dy);
	if (index == INVALID_INDEX) return 0; //Index out of range, something wrong

	//Target must be in AIR
	if (world->grid[index].type != AIR) return 0;

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
	return 0;
}

bool cell::cellSimilarityTest(cell* target){

	if (target == NULL){
		std::cout << "CELL SIMILARITY TEST NULL POINTER ERROR, EXITING" << std::endl;
		exit(0);
	}

	//Target must be pretty similar to giver
	int tr = target->DNA->color_red;
	int tg = target->DNA->color_green;
	int tb = target->DNA->color_blue;
	int r = DNA->color_red;
	int g = DNA->color_green;
	int b = DNA->color_blue;
	int difference = abs(tr - r) + abs(tg - g) + abs(tb - b);

	return (difference < energy_transfer_likeness_limit);

}

void cell::energy_transfer()
{
	//It cant happen all the time, dice rolls
	if (energy < energy_transfer_limit || rand() % 100 > DNA->energy_transfer_rate) return;

	//Checks if chosen target is still alive
	int parent_index = world->vectorToIndex(mergedCellParentPosX,mergedCellParentPosY);
	int child_index = world->vectorToIndex(mergedCellChildPosX,mergedCellChildPosY);

	//Cleaning up just in case
	if (parent_index == INVALID_INDEX)
	{
		mergedCellParentPosX = INVALID_COORDINATE;
		mergedCellParentPosY = INVALID_COORDINATE;
	}

	if (child_index == INVALID_INDEX)
	{
		mergedCellChildPosX = INVALID_COORDINATE;
		mergedCellChildPosY = INVALID_COORDINATE;
	}

	//Finding parent and cell
	cell* parent_cell = NULL;
	cell* child_cell = NULL;
	if (parent_index != INVALID_INDEX) parent_cell = world->grid[parent_index].life;
	if (child_index != INVALID_INDEX) child_cell = world->grid[parent_index].life;


	//Sharing energy
	if (parent_cell == NULL && child_cell == NULL){ //No merged cells
		return;
	} else if (parent_cell == NULL){ //Only child cell

		std::cout << "Sharing energy, with child" << std::endl;
		int sum = energy + child_cell->energy;
		energy = floor(sum/2);
		child_cell->energy = floor(sum/2);

	} else if (child_cell == NULL){ //Only parent cell

		std::cout << "Sharing energy, with parent" << std::endl;
		int sum = energy + parent_cell->energy;
		energy = floor(sum/2);
		parent_cell->energy = floor(sum/2);

	} else{ //Both

		std::cout << "Sharing energy, with parent and child" << std::endl;
		int sum = energy + parent_cell->energy + child_cell->energy;
		energy = floor(sum/3);
		parent_cell->energy = floor(sum/3);
		child_cell->energy = floor(sum/3);
	}
}

void cell::duplicate()
{
	//Checks energylevels
	if (energy < breedingHungerLimit) return;

	//Checks breedchance
	if(rand()%100 > DNA->breedrate[DNA->foodtype]) return;

	//Gets random direction
	int dx;
	int dy;
  getRandomDirection(DNA->preferred_duplication_dir, dx, dy); //Gives 8 as an argument so we do not get a preferred directio sim

	//Checks if choosen spawn spot is valid
	int index = world->vectorToIndex(xpos+dx,ypos+dy);
	if (index == INVALID_INDEX) return;

	//Cant dig into ground unless DNA allows it to
	if (world->grid[index].type != AIR)
	{
		if (!DNA->digger) return;
	}

	//Plants dont kill to make room for its child
	if (world->grid[index].life != NULL && DNA->foodtype == 0) return;

	//Other will
	if (world->grid[index].life != NULL)
	{
		delete world->grid[index].life;
		world->grid[index].life = NULL;
	}

	world->grid[index].life = new cell(world);
	cell* child = world->grid[index].life;

	child->xpos = xpos + dx;
	child->ypos = ypos + dy;
	child->xvel = xvel;
	child->yvel = yvel;
	child->energy = floor(energy/2);
	delete child->DNA;
	child->DNA = new DNAOBJECT(DNA);

	energy = floor(energy/2);
}

int cell::crawl()
{

	//Only moves if dna allows for it
	if (!DNA->stationary) return 0;

	//Only does this if not hungry
	if (energy < movementHungerLimit) return 0;

	//Changes direction
	if (rand() % 100 < DNA->turnfreq)
	{
		//Gets random direction
		int xvel;
		int yvel;
		getRandomDirection(8, xvel, yvel); //Gives 8 as an argument so we do not get a preferred directio sim
	}

	//Moves
	if (rand() % 100 < DNA->movefreq)
	{
		if (world->isFreeAir(xpos+xvel,ypos+yvel))
		{
			world->moveToLocation(world->vectorToIndex(xpos,ypos),xpos+xvel,ypos+yvel);
			energy -= movementCost;
			return 1;
		}
	}

	return 0;
}

void cell::getRandomDirection(int pref, int &dx, int &dy)
{
	int dir = rand() % 8;
	if (pref != 8 && rand() % 2) dir = pref; //Simulates preferred direction

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
}

cell::~cell()
{
	delete DNA;
}
