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
   		window->draw(circle);
   		
    }
}
