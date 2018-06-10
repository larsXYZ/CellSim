//MODULES
#include "world.h"
#include "cell.h"
#include "SETTINGS.h"
#include "functions.h"

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>




int main()
{
  //Starts SFML graphics window
  sf::RenderWindow window(sf::VideoMode(xsize*gridsize, ysize*gridsize), "CellSim");

	//Seeds random number generator
	srand(time(NULL));

	//Activates modules
	worldObject* world = new worldObject;

    while (window.isOpen()){

		  //SIMULATION
  		world->update();

    	  //EVENTHANDLING
        sf::Event event;
        while (window.pollEvent(event)){

          if (event.type == sf::Event::Closed)
                window.close();

          if (event.type == sf::Event::KeyPressed)

          if (event.key.code == sf::Keyboard::Escape){

            delete world;
  					world = new worldObject;
  				}
        }

		  //GRAPHICS
		  drawWorld(world, &window);
		  drawLife(world, &window);
      window.display();
      window.clear();
    }

    return 0;
}
