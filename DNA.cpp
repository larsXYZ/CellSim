#include "DNA.h"
#include "SETTINGS.h"

#include <stdlib.h>

DNAOBJECT::DNAOBJECT()
{
	color_red = rand() % 100 + 156;
	color_green = rand() % 100 + 156;
	color_blue = rand() % 100 + 156;
	
	breedrate[0] = rand() % (breedrate_plant_max-breedrate_plant_min) + breedrate_plant_min;
	breedrate[1] = rand() % (breedrate_herbavore_max-breedrate_herbavore_min) + breedrate_herbavore_min;
	lifespan = rand() % (lifespan_max-lifespan_min) + lifespan_min;
	movefreq = rand() % (movefreq_max-movefreq_min) + movefreq_min;
	stationary = rand() % 2;
	turnfreq = rand() % (turnfreq_max-turnfreq_min) + turnfreq_min;
	foodtype = rand() % 2;
	
}


DNAOBJECT::DNAOBJECT(DNAOBJECT *d)
{
	color_red = d->color_red + (rand()%mutation_rate-mutation_rate/2);
	color_green = d->color_green + (rand()%mutation_rate-mutation_rate/2);
	color_blue = d->color_blue + (rand()%mutation_rate-mutation_rate/2);
	
	breedrate[0] = d->breedrate[0] +(rand()%mutation_rate-mutation_rate/2);
	breedrate[1] = d->breedrate[1] +(rand()%mutation_rate-mutation_rate/2);
	lifespan = d->lifespan +(rand()%mutation_rate-mutation_rate/2);
	movefreq = d->movefreq +(rand()%mutation_rate-mutation_rate/2);
	turnfreq = d->turnfreq +(rand()%mutation_rate-mutation_rate/2);
	
	foodtype = d->foodtype;
	
	if (rand() % 100 < mutation_rate) stationary = !stationary;
	if (rand() % 3000 < mutation_rate) foodtype = rand() % 2;
	
}
