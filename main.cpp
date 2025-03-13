#include <cmath>
#include <vector>

#include "ForwardDeclaration.h"
#include "graphic.h"
#include "line.h"

int main() {
  Setup setup(getParametersFromUser());

  sf::RenderWindow window;

  sf::CircleShape ball(1);

  System system(makeSystemFromSetup(setup));

  Point interception_top_line{
      findInterception(system.top_line, system.first_throw)};
  Point interception_bottom_line{
      findInterception(system.bottom_line, system.first_throw)};
  Point last_interception{0, 0};
  Point new_interception{calculateFirstHit(
      interception_top_line, interception_bottom_line, setup, system)};

  window.create(sf::VideoMode(800, 600), "Biliardo");
  std::vector<Point> positions{};
  Point final_point{getFinalPoint(new_interception, last_interception, system,
                                  setup, positions)};

  openWindow(window, ball, 25 * setup.l, 25 * setup.r_1, 25 * setup.r_2,
             25 * setup.y_0, positions);

  getNormalDistribution(setup);

  return 0;
}

/*cosa dobbiamo fare:
ottimizzare,
migliorare leggibilità,
toglioere file inutili,
implementare bottoni e imput a schermo e non aq terminale, così come gli output,
facoltativo: implementare linea tratteggiata per percorso,
separare parte gaussiane con resto,
bisognerebbe introdurre la variabile di scala e speed, le usiamo troppo spesso,
possibilità di salvare i grafici da qualche altra parte,
miglorare nomi. */
