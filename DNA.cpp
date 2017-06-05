#include "DNA.h"
#include "SETTINGS.h"

#include <stdlib.h>

DNAOBJECT::DNAOBJECT()
{
	color_red = rand() % 256;
	color_green = rand() % 256;
	color_blue = rand() % 256;
	
	breedrate = rand() % (breedrate_max-breedrate_min) + breedrate_min;
	lifespan = rand() % (lifespan_max-lifespan_min) + lifespan_min;
}


DNAOBJECT::DNAOBJECT(DNAOBJECT *d)
{
	color_red = d->color_red + (rand()%mutation_rate-mutation_rate/2);
	color_green = d->color_green + (rand()%mutation_rate-mutation_rate/2);
	color_blue = d->color_blue + (rand()%mutation_rate-mutation_rate/2);
	
	breedrate = d->breedrate +(rand()%mutation_rate-mutation_rate/2);
	lifespan = d->lifespan +(rand()%mutation_rate-mutation_rate/2);
}
