#include "DNA.h"
#include "SETTINGS.h"

#include <stdlib.h>

DNAOBJECT::DNAOBJECT()
{
	color_red = rand() % 100 + 156;
	color_green = rand() % 100 + 156;
	color_blue = rand() % 100 + 156;
	
	breedrate = rand() % (breedrate_max-breedrate_min) + breedrate_min;
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
	
	breedrate = d->breedrate +(rand()%mutation_rate-mutation_rate/2);
	lifespan = d->lifespan +(rand()%mutation_rate-mutation_rate/2);
	movefreq = d->movefreq +(rand()%mutation_rate-mutation_rate/2);
	turnfreq = d->turnfreq +(rand()%mutation_rate-mutation_rate/2);
	
	foodtype = d->foodtype;
	
	if (rand() % 100 < mutation_rate) stationary = !stationary;
	if (rand() % 2000 < mutation_rate) foodtype = rand() % 2;
	
}
