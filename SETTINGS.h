
// SETTINGS.h
#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

//Foodtypes
#define ISPLANT 0
#define HERBAVORE 1
#define CARNIVORE 2

#define INVALID_INDEX -1
#define INVALID_COORDINATE -1

//Gridsize
static const int gridsize = 10;	//Size of gridcell in pixels

//Debug
static const bool HIGHLIGHT_MERGED = false;
static const bool HIGHLIGHT_GROUND = false;


//World settings
enum gridtype{DIRT,AIR,STONE}; //Types of gridcell
static const int xsize = 200;	//Number of gridcells horizontal
static const int ysize = 100;	//Number of gridcells vertical
static const int sunStrength = 3; //First cell gets +sunStrength from photosynthesis, second gets sunStrength-1, etc
static const double minLightCoeff = 0.8; //Used in printworld, for it not to be to dark
static const double minEnergyCoeff = 0.6;
static const int cell_max_energy = 100;
static const int start_life_spread = 20;
static const int ground_nutrient_max = 5;
static const int ground_nutrient_min = 2;

//DNA settings
static const int mutation_rate = 3;
static const int breedrate_plant_min = 1;
static const int breedrate_plant_max = 25;
static const int breedrate_herbavore_min = 5;
static const int breedrate_herbavore_max = 20;
static const int breedrate_carnivore_min = 5;
static const int breedrate_carnivore_max = 20;
static const int lifespan_min = 2000;
static const int lifespan_max = 4000;
static const int movefreq_min = 1;
static const int movefreq_max = 50;
static const int turnfreq_min = 1;
static const int turnfreq_max = 10;
static const int energy_transfer_rate_min = 50;
static const int energy_transfer_rate_max = 100;
static const int merge_rate_min = 0;
static const int merge_rate_max = 20;

//Cells
static const int movementCost = 2;
static const int photosynthesisStrength = 2;
static const int lifeCost = 1;
static const int eatChancePlant = 10;
static const int eatChanceAnimal = 50;
static const int movementHungerLimit = 20;
static const int eatingHungerLimit = 50;
static const int breedingHungerLimit = 60;
static const int energy_transfer_limit = 60;
static const int energy_transfer_likeness_limit = 30;
static const int min_soil_nutrient_amount_for_merge = 2;
static const int standard_soil_nutrient_gain = 100;
static const int digger_soil_nutrient_gain = 5000;


#endif
