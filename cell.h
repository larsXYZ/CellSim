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
	int soil_nutrients;
	int age;

	int mergedCellChildPosX;
	int mergedCellChildPosY;

	int mergedCellParentPosX;
	int mergedCellParentPosY;

	//Constructor
	cell();
	cell(worldObject* w);

	int live(); //Returns 1 if cell has moved
	void duplicate();
	int eatPlant(); //Returns 1 if it eats something
	int crawl(); //Returns 1 if it moves
	void energy_transfer();
	int eatAnimal();
	void photosynthesis();
	void absorbSoilEnergy();
	void tryToMergeChild();
	void tryToMergeParent(); //Look for a cell to merge with, to share energy with
	void notifyChild(cell* target); //Tell the cell that you are parent
	void notifyParent(cell* target); //Tell the cell that you are parent
	bool cellSimilarityTest(cell* target); //Tests if target is similar enough for action
	bool isMerged();
	bool hasChild();
	bool hasParent();

	void getRandomDirection(int pref, int &dx, int &dy);

	~cell();

};
