
struct DNAOBJECT
{
	int color_red;
	int color_green;
	int color_blue;

	int breedrate; // 0 - 50, % chance of breeding when possible
	int lifespan;  //Time till death by age
	
	//Constructor
	DNAOBJECT();
	DNAOBJECT(DNAOBJECT *d);
};
