struct worldObject;
struct DNAOBJECT;

struct cell
{

	worldObject* world;
	DNAOBJECT *DNA;
	
	int xpos;
	int ypos;
	int energy;
	int age;

	
	//Constructor
	cell();
	cell(worldObject* w);
	
	void live();
	void duplicate();

};

