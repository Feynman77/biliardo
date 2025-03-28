

#include <SFML/System/Clock.hpp>

#include "ForwardDeclaration.h"

int main() {
  // initial setting
  sf::RenderWindow window;
  tgui::Gui gui{window};
  fillGui(gui);

  Setup setup;

  bool throw_pressed = false;
  bool normal_distribution_pressed = false;

  float scale{25};
  float speed{2};

  unsigned int i{0};

  std::vector<Point> positions{};

  // creation of the ball
  sf::CircleShape ball(1);

  sf::VertexArray top_line(sf::Lines, 2);
  sf::VertexArray bottom_line(sf::Lines, 2);
  sf::VertexArray hor_line(sf::Lines, 2);
  sf::VertexArray vert_line(sf::Lines, 2);
  sf::Texture texture;
  sf::Sprite sprite;

  setWindow(window, ball, hor_line, vert_line, sprite);

  gui.get<tgui::Button>("gauss")->onClick(
      [&]() { normal_distribution_pressed = true; });
  gui.get<tgui::Button>("run")->onClick([&]() { throw_pressed = true; });

  while (window.isOpen()) {
    sf::Event event;
    // check all the window's events that were triggered since the last
    // iteration of the loop
    float r_1 = gui.get<tgui::EditBoxSlider>("r_1")->getValue();
    gui.get<tgui::EditBoxSlider>("y_0")->setMaximum(r_1 - 0.01f);
    gui.get<tgui::EditBoxSlider>("y_0")->setMinimum(-r_1 + 0.01f);

    while (window.pollEvent(event)) {
      gui.handleEvent(event);
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::Resized) {
        window.setSize(sf::Vector2u(1400, 950));
      }
    }

    if (throw_pressed == true) {
      ball.setFillColor(sf::Color::Green);  // why isn't it green???
      ball.setOutlineColor(sf::Color::Green);

      positions.clear();
      setup = (getParametersFromUser(gui));
      float scale_reference =
          std::max({setup.l / 30, setup.r_1 / 8, setup.r_2 / 8});
      scale = 25 / scale_reference;
      speed = 1.5f * scale_reference;
      makeDrawableSystem(ball, top_line, bottom_line, setup, scale);
      System system(makeSystemFromSetup(setup));

      Point last_interception{0, 0};
      Point new_interception{calculateFirstHit(setup.l, system)};

       getFinalPoint(new_interception, last_interception, system, setup.l,
                    positions, speed, scale, gui);

      i = 0;
      throw_pressed = false;
    };

    if (i < positions.size() && positions.size() != 0) {
      ball.setPosition(static_cast<float>(positions[i].x),
                       static_cast<float>(positions[i].y));
      i++;
    }

    if (i >= positions.size() && positions.size() != 0) {
      positions.clear();
      i = 0;
    }

    if (normal_distribution_pressed == true) {
      setup = (getParametersFromUser(gui));
      getNormalDistribution(setup, gui);

      normal_distribution_pressed = false;
      texture.loadFromFile("histos.png");
      sprite.setTexture(texture);
    };

    window.clear();
    window.draw(hor_line);
    window.draw(vert_line);
    window.draw(top_line);
    window.draw(bottom_line);
    window.draw(ball);
    window.draw(sprite);
    gui.draw();
    window.display();
  }
  return 0;
}

/*cosa dobbiamo fare:

ottimizzare, migliorare leggibilità, togliere file inutili,
implementare bottoni e imput a schermo e non a terminale, così come gli output,
facoltativo: implementare linea tratteggiata per percorso,
separare parte gaussiane con resto,

possibilità di salvare i grafici da qualche altra parte,
miglorare nomi.

creazione funzione per disegnare le linee dentro il while

problema: in getfinalpoint calcola, per motivi a noi ignoti, angolo e posizione
finale girati di segno

*/
