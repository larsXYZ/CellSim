
// SETTINGS.h
#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

//Gridsize
static const int gridsize = 5;	//Size of gridcell in pixels

//World settings
enum gridtype{DIRT,AIR,STONE}; //Types of gridcell
static const int xsize = 360;	//Number of gridcells horizontal
static const int ysize = 200;	//Number of gridcells vertical
static const int sunStrength = 3; //First cell gets +sunStrength from photosynthesis, second gets sunStrength-1, etc
static const double minLightCoeff = 0.6;

//DNA settings
static const int mutation_rate = 5;
static const int breedrate_plant_min = 1;
static const int breedrate_plant_max = 20;
static const int breedrate_herbavore_min = 1;
static const int breedrate_herbavore_max = 3;


static const int lifespan_min = 50;
static const int lifespan_max = 1000;
static const int movefreq_min = 1;
static const int movefreq_max = 100;
static const int turnfreq_min = 1;
static const int turnfreq_max = 10;

//Cells
static const int movementCost = 4;
static const int photosynthesisStrength = 1;
static const int lifeCost = 1;
static const int eatChancePlant = 10;

#endif
