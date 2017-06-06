struct worldObject;
struct DNAOBJECT;

struct cell
{

	worldObject* world;
	DNAOBJECT *DNA;
	
	int xpos;
	int ypos;
	
	int xvel;
	int yvel;
	
	int energy;
	int age;

	
	//Constructor
	cell();
	cell(worldObject* w);
	
	void live();
	void duplicate();
	void crawl(int i);
	
	int* getRandomDirection();

};

