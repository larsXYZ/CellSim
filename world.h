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
	void moveToLocation(int i, int x, int y);
	
	//Helper functions
	int vectorToIndex(int x, int y);
	int* indexToVector(int i);
	bool isFree(int x, int y);
	
	//Destructor
	~worldObject();
};
