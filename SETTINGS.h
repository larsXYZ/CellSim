
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
static const int start_life_spread = 20;
static const int ground_nutrient_max = 3;
static const int ground_nutrient_recovery_rate = 1;

//DNA settings
static const int mutation_rate = 3;
static const int breedrate_plant_min = 1;
static const int breedrate_plant_max = 25;
static const int breedrate_herbavore_min = 5;
static const int breedrate_herbavore_max = 20;
static const int breedrate_carnivore_min = 5;
static const int breedrate_carnivore_max = 20;
static const int lifespan_min = 50;
static const int lifespan_max = 1000;
static const int movefreq_min = 1;
static const int movefreq_max = 50;
static const int turnfreq_min = 1;
static const int turnfreq_max = 10;
static const int energy_transfer_rate_min = 30;
static const int energy_transfer_rate_max = 60;


//Cells
static const int movementCost = 4;
static const int photosynthesisStrength = 1;
static const int soilentEnergyStrength = 2;
static const int lifeCost = 1;
static const int eatChancePlant = 50;
static const int eatChanceAnimal = 50;
static const int movementHungerLimit = 20;
static const int eatingHungerLimit = 50;
static const int breedingHungerLimit = 60;
static const int energy_transfer_amount = 20;
static const int energy_transfer_limit = 60;
static const int energy_transfer_likeness_limit = 30;


#endif
