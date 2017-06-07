struct worldObject;
struct DNAOBJECT;

struct cell
{

	worldObject* world;
	DNAOBJECT *DNA;
	
	bool hasMoved;
	
	int xpos;
	int ypos;
	
	int xvel;
	int yvel;
	
	int energy;
	int age;

	
	//Constructor
	cell();
	cell(worldObject* w);
	
	int live(); //Returns 1 if cell has moved
	void duplicate();
	int eatPlant(); //Returns 1 if it eats something
	int crawl(); //Returns 1 if it moves
	void energy_transfer();
	int eatAnimal();
	
	int* getRandomDirection(int pref);

};

