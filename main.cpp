#include <cmath>

#include "ForwardDeclaration.h"
#include "graphic.h"
#include "line.h"

// int main() {
/* Setup setup(getParametersFromUser());

 System system(makeSystemFromSetup(setup));

 Point interception_top_line{
     findInterception(system.top_line, system.first_throw)};
 Point interception_bottom_line{
     findInterception(system.bottom_line, system.first_throw)};
 Point last_interception{0, 0};
 Point new_interception{calculateFirstHit(
     interception_top_line, interception_bottom_line, setup, system)};

 Point final_point{
     getFinalPoint(new_interception, last_interception, system, setup)};

 getNormalDistribution(setup); */

int main() {
  Setup setup(getParametersFromUser());

  double speed{10};
  sf::RenderWindow window;
  window.setFramerateLimit(30);
  sf::CircleShape ball(3);

  // dobbiamo riscalare io propongo tipo moltiplicando per 25
 

  System system(makeSystemFromSetup(setup));

  Point interception_top_line{
      findInterception(system.top_line, system.first_throw)};
  Point interception_bottom_line{
      findInterception(system.bottom_line, system.first_throw)};
  Point last_interception{0, 0};
  Point new_interception{calculateFirstHit(
      interception_top_line, interception_bottom_line, setup, system)};

      
      window.create(sf::VideoMode(800, 600), "Biliardo");
  Point final_point{
      getFinalPoint(new_interception, last_interception, system, setup, ball, speed, window)
    };


    return 0;
}
