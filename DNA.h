
struct DNAOBJECT
{
	int color_red;
	int color_green;
	int color_blue;

	int breedrate; // 0 - 50, % chance of breeding when possible
	int lifespan;  //Time till death by age
	int stationary; //0 - 1
	int movefreq; //How often a creature moves
	int turnfreq; //How often a creature changes direction
	
	//Constructor
	DNAOBJECT();
	DNAOBJECT(DNAOBJECT *d);
};
