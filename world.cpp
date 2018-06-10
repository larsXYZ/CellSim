#include "world.h"
#include "cell.h"
#include "DNA.h"

#include <iostream>
#include <cmath>

worldObject::worldObject()
{
	generate_ground();
	generate_life();
}

void worldObject::generate_ground()
{
	//Clears all existing tiless
	for (int i = 0; i < xsize*ysize; i++) grid[i].type = AIR;

	//Generates stone layer
	int h = (ysize/6);
	int dh = 0;
	int dh2 = 0;
	for (int i = 0; i < xsize; i++)
	{
		for (int q = 0; q < h; q++)
			grid[(ysize-q-1)*xsize + i].type = STONE;
			dh = rand() % 3 - 1;
		if (i % 7 == 0)
			dh2 = rand() % 3 - 1;

		if (h + dh + dh2 < ysize && h + dh + dh2 > 5)
			h += (dh + dh2);
	}

	//Generates dirt layer
	h = (ysize/2);
	dh = 0;
	dh2 = 0;
	for (int i = 0; i < xsize; i++)
	{
		int q = h;
		while (grid[(ysize-q-1)*xsize + i].type != STONE)
		{
			grid[(ysize-q-1)*xsize + i].type = DIRT;
			q--;
		}
		dh = rand() % 3 - 1;
		if (i % 8 == 0) dh2 = rand() % 3 - 1;
		if (h + dh + dh2 < ysize && h + dh + dh2 > 0 && grid[(ysize-h+2)*xsize + i].type != STONE) h += (dh + dh2);
	}

	//Generates nutrient veins
	int rad = 5;
	for (int x = 0; x < (xsize-1); x += 2)
	{
		for (int y = 0; y < (ysize-1); y += 2)
		{
			int n_val = rand() % ground_nutrient_max;
			if (n_val <= ground_nutrient_min) n_val = 0;

			for (int fx = -rad; fx <= rad; fx++)
			{
				for (int fy = -rad; fy <= rad; fy++)
				{
					if ((x + fx > 0) && (x + fx < xsize) && (y + fy > 0) && (y + fy < ysize))
					{
						grid[((y+fy)*xsize)+x+fx].nutrients = n_val;
					}
				}
			}
		}
	}

	for (int x = 0; x < (xsize-1); x += 1)
	{
		for (int y = 0; y < (ysize-1); y += 1)
		{
			if (grid[(y)*xsize + x].nutrients != 0) continue;

			int n_val = rand() % ground_nutrient_max;
			if (n_val <= ground_nutrient_min) n_val = 0;

			if (n_val < 1) n_val = 0;
			grid[(y)*xsize + x].nutrients = n_val;
		}
	}

	//Generates caves
	for (int i = 0; i < 10; i++)
	{
		int x = (i*xsize/12)+xsize/12+(rand()%5-2);
		int dx = 0;
		int y = 0;
		while (grid[((y+2)*xsize)+x].type == AIR) y++;

		int rad = rand() % 6 + 1;
		while (rad > 1)
		{
			for (int fx = -rad; fx <= rad; fx++)
			{
				for (int fy = -rad; fy < -1; fy++)
				{
					if ((x + fx > 0) && (x + fx < xsize) && (y + fy > 0) && (y + fy < ysize))
					{
						grid[((y+fy)*xsize)+x+fx].type = AIR;
					}
				}
			}
			if (rad%3 == 0) dx = rand() % 7 - 3;
			x += rand() % 5 - 2 + dx;
			if (rand() % 100 < (((float)y/ysize)*8)) rad -= rand() % 3 + 1;
			y += 1;
			if (y > ysize) break;
		}
	}
}

void worldObject::update()
{

	//Resets all lighting to zero and resets all lifeforms and regains nutrients
	for(int i = 0; i < xsize*ysize; i++)
	{
		grid[i].lightStrength = 0;
		if (grid[i].life != NULL) grid[i].life->hasMoved = false;

	}
	//Calculates light
	calcLight();

	//Cell stuff
	for (int i = xsize*ysize-1; i >= 0 ; i--)
	{

		if (grid[i].life != NULL)
		{
			cell* cell = grid[i].life;

			//Checks if the cell already has done something
			if (cell->hasMoved) continue;
			cell->hasMoved = true;

			//Check for death
			if (cell->energy <= 0 || cell->age > cell->DNA->lifespan)
			{
				delete grid[i].life;
				grid[i].life = NULL;
				continue;
			}

			//Lets cell live
			if (cell->live()) continue;

		}
	}

	gravity();
}

void worldObject::calcLight()
{

	for(int x = 0; x < xsize; x++)
	{
		int y = 0;
		int beamStrength = sunStrength;

		while (beamStrength > 0)
		{
			int index = vectorToIndex(x,y);
			if (index == INVALID_INDEX) std::cout << "CALCLIGHTERROR\n";

			//Cell lights up
			grid[index].lightStrength = beamStrength;

			//Hit detection /GROUND /LIFE
			if(grid[index].type != AIR) beamStrength = 0;

			//Hit detection /LIFE
			if (grid[index].life != NULL) beamStrength--;

			y++;
		}
	}


}

void worldObject::generate_life()
{
	for (int x = 0; x < xsize/start_life_spread; x++)
	{
		cell* new_cell = new cell(this);
		new_cell->xpos = x*start_life_spread + 10;

		int y = 0;
		while(grid[vectorToIndex(x*start_life_spread,y)].type == AIR) y++;

		new_cell->ypos = y;
		new_cell->DNA->foodtype = 0; //We must start with plants
		grid[vectorToIndex(new_cell->xpos,new_cell->ypos)].life = new_cell;
	}
}

void worldObject::moveToLocation(int i, int x, int y)
{

	int next_index = vectorToIndex(x,y);
	if (next_index == INVALID_INDEX)
	{
		std::cout << "MOVETOLOCATION ERROR\n";
		return;
	}

	grid[next_index].life = grid[i].life;
	grid[next_index].life->xpos = x;
	grid[next_index].life->ypos = y;
	grid[i].life = NULL;
}

bool worldObject::isFreeAir(int x, int y)
{
	int index = vectorToIndex(x,y);

	if (index == INVALID_INDEX)
	{
		std::cout << "isAir ERROR\n";
		return 0;
	}

	gridcell cell_below = grid[index];
	return (cell_below.type == AIR)&&(cell_below.life == NULL);
}

bool worldObject::isFreeDirt(int x, int y)
{
	int index = vectorToIndex(x,y);

	if (index == INVALID_INDEX)
	{
		std::cout << "isAir ERROR\n";
		return 0;
	}

	gridcell cell_below = grid[index];
	return (cell_below.type == DIRT)&&(cell_below.life == NULL);
}

int worldObject::vectorToIndex(int x, int y)
{
	if (x < 0 || x >= xsize || y < 0 || y >= ysize)
	{
		//std::cout << "OUT OF BOUNDS VECTORTOINDEX\n" << x << " " << y << std::endl;
		return INVALID_INDEX; //ERROR OUT OF BOUNDS
	}

	return y*xsize + x;
}

int* indexToVector(int i)
{
	if (i > xsize*ysize)
	{
		std::cout << "OUT OF BOUNDS INDEXTOVECTOR\n";
		exit(0);
	}

	std::cout << "INDEXTOVECTOR IS USED";

	int result[2] = {0,0};
	result[0] = i % xsize;
	result[1] = i / xsize;

	return result;
}

void worldObject::gravity(){

	//Generating groundmap
	bool groundmap[ysize*xsize];
	for (int i = 0; i < ysize*xsize; i++) groundmap[i] = false;

	//Recursive look for ground structures
	for (int y = ysize-1; y >= 0; y--){
		for (int x = 0; x < xsize; x++){

			int index = vectorToIndex(x,y);
			if (index == INVALID_INDEX) continue;

			if ((grid[index].type == DIRT || grid[index].type == STONE) && !groundmap[index]){
				localGroundRecursive(x,y,groundmap);
			}
		}
	}

	//Starting from the bottom row, going up
	for (int y = ysize-2; y >= 0; y--){
		for (int x = 0; x < xsize; x++){

				//Cells fall
				int index = vectorToIndex(x,y);
				if (grid[index].life == NULL) continue;
				if (isFreeAir(x,y+1) && !groundmap[index]) moveToLocation(vectorToIndex(x,y),x,y+1);

			}
		}
}


void worldObject::localGroundRecursive(int x, int y, bool *groundmap){

	int index = vectorToIndex(x,y);
	if (index == INVALID_INDEX) return;
	groundmap[index] = true;

	//Checking to all sides
	for (int dx = -1; dx <= 1; dx++){
		for (int dy = 1; dy >= -1; dy--){

			//No diagonals
			if (dx == dy) continue;

			int target_index = vectorToIndex(x+dx,y+dy);

			if (target_index == INVALID_INDEX) continue;
			if (groundmap[target_index]) continue;

			cell* target_cell = grid[target_index].life;

			if (target_cell != NULL){

				if (grid[target_index].type == AIR){

					if (target_cell->isMerged()){
						localGroundRecursive(x+dx,y+dy,groundmap);
					} else {
						groundmap[target_index] = true;
					}
				}

			}
		}
	}
}

worldObject::~worldObject()
{
	for (int i = 0; i < xsize*ysize; i++)
	{
		delete grid[i].life;
		grid[i].life = NULL;
	}
}
