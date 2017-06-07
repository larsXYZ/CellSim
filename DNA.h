
struct DNAOBJECT
{
	int color_red;
	int color_green;
	int color_blue;

	int breedrate[3]; // 0 - 50, % chance of breeding when possible
	int lifespan;  //Time till death by age
	int stationary; //0 - 1
	int movefreq; //How often a creature moves
	int turnfreq; //How often a creature changes direction
	int foodtype;  // 0 - photosynthesis, 1 - herbavore, 2 - carnivore, 3 - omnivore
	int preferred_duplication_dir; // 0 - 8, 8 -> no pref
	int energy_transfer_level; //When it will share energy
	int energy_transfer_rate; //Chance that cell will give another cell some of its energy, if it has plenty
	
	//Constructor
	DNAOBJECT();
	DNAOBJECT(DNAOBJECT *d);
};
