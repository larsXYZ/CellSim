#include "SETTINGS.h"

#include <stdlib.h>

struct cell;
struct gridcell
{
	cell* life = NULL;
	gridtype type = AIR;

	int nutrients = 0;
	int lightStrength = 0; //Currently sunStrength - 0
};

struct worldObject
{
	gridcell grid[xsize*ysize];

	//Constructors
	worldObject();

	//Startup functions
	void generate_ground();
	void generate_life();

	//Simulation functions
	void update();
	void calcLight();

	//Gravity functions
	void gravity();
	void localGroundRecursive(int x, int y, bool *groundmap);


	//Life functions
	void moveToLocation(int i, int x, int y);

	//Helper functions
	int vectorToIndex(int x, int y);
	int* indexToVector(int i);
	bool isFreeAir(int x, int y);
	bool isFreeDirt(int x, int y);

	//Destructor
	~worldObject();
};
