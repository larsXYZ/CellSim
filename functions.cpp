#include "functions.h"
#include "world.h"
#include "cell.h"
#include "SETTINGS.h"
#include "DNA.h"

void drawWorld(worldObject* world, sf::RenderWindow* window)
{
	for(int i = 0; i < xsize*ysize; i++)
    {
    	int x = i%xsize;
    	int y = i/xsize;

    	sf::RectangleShape rectangle(sf::Vector2f(gridsize, gridsize));
    	rectangle.setPosition(x*gridsize,y*gridsize);

   		switch (world->grid[i].type)
   		{
   			case DIRT:
    		{
    			rectangle.setFillColor(sf::Color(130,100,100));
    			//If the soil is not nutrient it will be slightly darker
    			if (world->grid[i].nutrients == 0) rectangle.setFillColor(sf::Color(125,95,95));
   				break;
   			}

   			case STONE:
   			{
   				rectangle.setFillColor(sf::Color(120,120,120));
   				break;
   			}
   			case AIR:
   			{
				rectangle.setFillColor(sf::Color(100,100,230));
    			break;
    		}
    	}

		//Grid might be in shadow
		if (world->grid[i].lightStrength < sunStrength)
		{
			double lightCoeff = minLightCoeff + (1-minLightCoeff)*((double)world->grid[i].lightStrength/(double)sunStrength);

			sf::Color nColor = rectangle.getFillColor();
			nColor.r = lightCoeff*nColor.r;
			nColor.g = lightCoeff*nColor.g;
			nColor.b = lightCoeff*nColor.b;
			rectangle.setFillColor(nColor);
		}

   		window->draw(rectangle);
    }
}

void drawLife(worldObject* world, sf::RenderWindow* window)
{
	for(int i = 0; i < xsize*ysize; i++)
    {
   		if (world->grid[i].life == NULL) continue;
   		cell* cell = world->grid[i].life;

   		int x = i%xsize;
    	int y = i/xsize;

    	sf::CircleShape circle;
			circle.setRadius(gridsize/2);
    	circle.setPosition(x*gridsize,y*gridsize);
   		circle.setFillColor(sf::Color(cell->DNA->color_red,cell->DNA->color_green,cell->DNA->color_blue));

   		//Cell might be in shadow or herbavore/carnivore
   		if (cell->DNA->foodtype == HERBAVORE) circle.setFillColor(sf::Color::Green);
   		else if (cell->DNA->foodtype == CARNIVORE) circle.setFillColor(sf::Color::Red);
			else if (world->grid[i].lightStrength < sunStrength)
			{
			double lightCoeff = minLightCoeff + (1-minLightCoeff)*((double)world->grid[i].lightStrength/(double)sunStrength);

			sf::Color nColor = circle.getFillColor();
			nColor.r = lightCoeff*nColor.r;
			nColor.g = lightCoeff*nColor.g;
			nColor.b = lightCoeff*nColor.b;

			//Cells with less energy are shaded
			float energyCoeff = (cell->energy)/cell_max_energy;
			if (energyCoeff < minEnergyCoeff) energyCoeff = minEnergyCoeff;
			nColor.r = energyCoeff*nColor.r;
			nColor.g = energyCoeff*nColor.g;
			nColor.b = energyCoeff*nColor.b;

			if (cell->isMerged()) nColor = sf::Color::Blue;

			circle.setFillColor(nColor);
		}



   		window->draw(circle);

    }
}
