#include <cmath>

#include "ForwardDeclaration.h"
#include "line.h"

int main() {
  Setup setup(getParametersFromUser());

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
  return 0;
}
