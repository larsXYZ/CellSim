#include "world.h"
#include "cell.h"
#include "DNA.h"

worldObject::worldObject()
{
	generate_ground();
	for (int i = 0; i < 5; i++) createLife();
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
		for (int q = 0; q < h; q++) grid[(ysize-q-1)*xsize + i].type = STONE;
		dh = rand() % 3 - 1;
		if (i % 7 == 0) dh2 = rand() % 3 - 1;
		if (h + dh + dh2 < ysize && h + dh + dh2 > 5) h += (dh + dh2);
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
	
	//Generates caves
	for (int i = 0; i < 7; i++)
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
	for (int i = xsize*ysize-1; i >= 0 ; i--)
	{
	
		if (grid[i].life != NULL)
		{
			cell* cell = grid[i].life;
			
			//Lets cell live
			cell->live();
			
			//Check for death
			if (cell->energy <= 0 || cell->age > cell->DNA->lifespan)
			{
				delete grid[i].life;
				grid[i].life = NULL;
				continue;
			}
			
			//Cells fall
			if (isFree(cell->xpos,cell->ypos+1))
			{
				grid[vectorToIndex(cell->xpos,++(cell->ypos))].life = cell;
				grid[i].life = NULL;
			}
			
			cell->crawl(i);
			
		}
		
	}
}

void worldObject::createLife()
{
	cell* new_cell = new cell(this);
	grid[(new_cell->ypos*xsize)+new_cell->xpos].life = new_cell;
}

void worldObject::moveToLocation(int i, int x, int y)
{
	grid[vectorToIndex(x,y)].life = grid[i].life;
	grid[i].life = NULL;
}

bool worldObject::isFree(int x, int y)
{
	gridcell cell_below = grid[vectorToIndex(x,y)];
	return (cell_below.type == AIR)&&(cell_below.life == NULL);
}

int worldObject::vectorToIndex(int x, int y)
{
	if (x < 0 || x >= xsize  || y < 0 || y >=ysize) return -1; //ERROR OUT OF BOUNDS
	return y*xsize + x;
}

int* indexToVector(int i)
{
	int result[2] = {0,0};
	result[0] = i % xsize;
	result[1] = i / xsize;
	
	return result;	
}

worldObject::~worldObject()
{
	for (int i = 0; i < xsize*ysize; i++)
	{
		delete grid[i].life;
	}
}








