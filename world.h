#include "SETTINGS.h"

#include <stdlib.h>

struct cell;
struct gridcell
{
	cell* life = NULL;
	gridtype type = AIR;
};

struct worldObject
{	
	gridcell grid[xsize*ysize];
	
	//Constructors
	worldObject();
	
	//Startup functions
	void generate_ground();
	
	//Simulation functions
	void update();
	
	//Life functions
	void createLife();
	bool canFall(cell* c);
	
	//Helper functions
	int vectorToIndex(int x, int y);
	int* indexToVector(int i);
	
	//Destructor
	~worldObject();
};
