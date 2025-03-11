#include <cmath>
#include <vector>

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

  double speed{25};
  sf::RenderWindow window;
  window.setFramerateLimit(30);
  sf::CircleShape ball(1);

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
      std::vector<Point> positions;
  Point final_point{
      getFinalPoint(new_interception, last_interception, system, setup, positions)
    };

    openWindow(window, ball, setup.l, setup.r_1, setup.r_2, setup.y_0, positions);


    return 0;
}
