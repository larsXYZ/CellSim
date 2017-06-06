
// SETTINGS.h
#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

//Gridsize
static const int gridsize = 5;	//Size of gridcell in pixels

//World settings
enum gridtype{DIRT,AIR,STONE}; //Types of gridcell
static const int xsize = 360;	//Number of gridcells horizontal
static const int ysize = 200;	//Number of gridcells vertical

//DNA settings
static const int mutation_rate = 5;
static const int breedrate_min = 1;
static const int breedrate_max = 30;
static const int lifespan_min = 50;
static const int lifespan_max = 1000;
static const int movefreq_min = 1;
static const int movefreq_max = 100;
static const int turnfreq_min = 1;
static const int turnfreq_max = 10;



#endif
