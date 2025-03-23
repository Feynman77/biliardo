#include <SFML/Window/Event.hpp>
#include <TGUI/Widgets/Slider.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#include "ForwardDeclaration.h"
#include "graphic.h"
#include "line.h"

int main() {
  // Setup setup(getParametersFromUser());

  sf::RenderWindow window;
  tgui::Gui gui{window};
  fillGui(gui);
  double scale{25};
  double speed{1};

  sf::CircleShape ball(1);
  window.create(sf::VideoMode(800, 600), "Biliardo");

  /*System system(makeSystemFromSetup(setup));

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
                                  setup, positions, speed, scale)};

  /*openWindow(window, ball, scale * setup.l, scale * setup.r_1,
             scale * setup.r_2, scale * setup.y_0, positions, gui)*/

  // getNormalDistribution(setup);

  sf::View view(sf::Vector2f(0, 0), sf::Vector2f(800, 600));
  window.setView(view);
  window.setFramerateLimit(60);

  // Calcola la posizione per centrarla
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  int posX = (desktop.width - window.getSize().x) / 2;
  int posY = (desktop.height - window.getSize().y) / 2;

  // Imposta la posizione della finestra
  window.setPosition(sf::Vector2i(posX, posY));
  sf::Clock clock;

  sf::VertexArray top_line(sf::Lines, 2);
  sf::VertexArray bottom_line(sf::Lines, 2);

  int i{0};

  std::vector<Point> positions{};

  bool button_is_pressed{false};
  gui.get<tgui::Button>("run")->onPress([&]() { button_is_pressed = true; });

  top_line[0].position = sf::Vector2f(0, 0);
  top_line[0].color = sf::Color::White;
  top_line[1].position = sf::Vector2f(0, 0);
  top_line[1].color = sf::Color::White;

  // creation of the bottom line

  bottom_line[0].position = sf::Vector2f(0, 0);
  bottom_line[0].color = sf::Color::White;
  bottom_line[1].position = sf::Vector2f(0, 0);
  bottom_line[1].color = sf::Color::White;


  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      gui.handleEvent(event);
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    
    if ((button_is_pressed = true)) {
      // run the program as long as the window is open
      while (window.isOpen()) {
        // check all the window's events that were triggered since the last
        // iteration of the loop
      

        while (window.pollEvent(event)) {
          gui.handleEvent(event);
          // "close requested" event: we close the window
          if (event.type == sf::Event::Closed) {
            window.close();
          }
        }

        gui.get<tgui::Button>("run")->onClick([&]() {
          positions.clear();
          Setup setup(getParametersFromUser(gui));
          makeDrawableSystem(ball, top_line, bottom_line, setup);
          System system(makeSystemFromSetup(setup));

          Point interception_top_line{
              findInterception(system.top_line, system.first_throw)};
          Point interception_bottom_line{
              findInterception(system.bottom_line, system.first_throw)};
          Point last_interception{0, 0};
          Point new_interception{calculateFirstHit(
              interception_top_line, interception_bottom_line, setup, system)};

          Point final_point{getFinalPoint(new_interception, last_interception,
                                          system, setup, positions, speed,
                                          scale)};
        });

        if (i < positions.size()) {
          ball.setPosition(positions[i].x, positions[i].y);
          i++;
        }

        window.clear();
        window.draw(top_line);
        window.draw(bottom_line);
        window.draw(ball);
        gui.draw();
        window.display();
      }
    }

    window.clear();
    gui.draw();
    window.display();
  }

  return 0;
}

// run the program as long as the window is open

/*cosa dobbiamo fare:
ottimizzare,
migliorare leggibilità,
toglioere file inutili,
implementare bottoni e imput a schermo e non aq terminale, così come gli output,
facoltativo: implementare linea tratteggiata per percorso,
separare parte gaussiane con resto,

possibilità di salvare i grafici da qualche altra parte,
miglorare nomi.

creazione funzione per disegnare le linee dentro il while*/

// run the program as long as the window is open
/*while (window.isOpen()) {
  // check all the window's events that were triggered since the last
  // iteration of the loop
  sf::Event event;

  while (window.pollEvent(event)) {
    gui.handleEvent(event);
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed) {
      window.close();
    }
  }

  gui.get<tgui::Button>("run")->onClick([&]() {
    positions.clear();
    Setup setup(getParametersFromUser(gui));
    makeDrawableSystem(ball, top_line, bottom_line, setup);
    System system(makeSystemFromSetup(setup));

    Point interception_top_line{
        findInterception(system.top_line, system.first_throw)};
    Point interception_bottom_line{
        findInterception(system.bottom_line, system.first_throw)};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(
        interception_top_line, interception_bottom_line, setup, system)};

    Point final_point{getFinalPoint(new_interception, last_interception,
                                    system, setup, positions, speed, scale)};
  });

  if (i < positions.size()) {
    ball.setPosition(positions[i].x, positions[i].y);
    i++;
  }

  window.clear();
  window.draw(top_line);
  window.draw(bottom_line);
  window.draw(ball);
  gui.draw();
  window.display();
} */