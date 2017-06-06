#include "SETTINGS.h"

#include <stdlib.h>

struct cell;
struct gridcell
{
	cell* life = NULL;
	gridtype type = AIR;
	int lightStrength = 0; //Currently sunStrength - 0
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
	void calcLight();
	
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
