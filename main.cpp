#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include <TGUI/Widgets/Slider.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "ForwardDeclaration.h"
#include "graphic.h"
#include "line.h"

int main() {
  // inizial setting
  sf::RenderWindow window;
  tgui::Gui gui{window};

  fillGui(gui);
  double scale{25};
  double speed{2};

  // creation of the ball
  sf::CircleShape ball(1);
  ball.setFillColor(sf::Color::Black);
  ball.setOutlineColor(sf::Color::Black);

  // drawing the window
  window.create(sf::VideoMode(1400, 950), "Biliardo",
                sf::Style::Titlebar | sf::Style::Close);
  sf::View view(sf::Vector2f(100, -225), sf::Vector2f(1400, 950));
  window.setView(view);
  window.setFramerateLimit(60);

  // centering the window
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  int posX = (desktop.width - window.getSize().x) / 2;
  int posY = (desktop.height - window.getSize().y - 100) / 2;

  // Imposta la posizione della finestra
  window.setPosition(sf::Vector2i(posX, posY));
  sf::Clock clock;

  sf::VertexArray top_line(sf::Lines, 2);
  sf::VertexArray bottom_line(sf::Lines, 2);

  int i{0};

  std::vector<Point> positions{};
  bool button_is_pressed{false};
  gui.get<tgui::Button>("run")->onPress([&]() { button_is_pressed = true; });

  // creation of the setup top line
  top_line[0].position = sf::Vector2f(0, 0);
  top_line[0].color = sf::Color::White;
  top_line[1].position = sf::Vector2f(0, 0);
  top_line[1].color = sf::Color::White;

  // creation of the setup  bottom line
  bottom_line[0].position = sf::Vector2f(0, 0);
  bottom_line[0].color = sf::Color::White;
  bottom_line[1].position = sf::Vector2f(0, 0);
  bottom_line[1].color = sf::Color::White;

  // creation of horizontal and vertical line for layout
  sf::VertexArray hor_line(sf::Lines, 2);
  sf::VertexArray vert_line(sf::Lines, 2);

  // creation horizontal line
  hor_line[0].position = sf::Vector2f(-800, -228);
  hor_line[0].color = sf::Color::White;
  hor_line[1].position = sf::Vector2f(800, -228);
  hor_line[1].color = sf::Color::White;

  // creation vertical line
  vert_line[0].position = sf::Vector2f(-50, -1000);
  vert_line[0].color = sf::Color::White;
  vert_line[1].position = sf::Vector2f(-50, 1000);
  vert_line[1].color = sf::Color::White;

  Setup setup;

  bool run_pressed = false;
  bool gaus_pressed = false;
  sf::Texture texture;
  sf::Sprite sprite;
  sprite.setPosition(-50, -700);
  sprite.setScale(1.23f, 1.0f);

  gui.get<tgui::Button>("gauss")->onClick([&]() { gaus_pressed = true; });
  gui.get<tgui::Button>("run")->onClick([&]() { run_pressed = true; });

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      gui.handleEvent(event);
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

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
    }

    if (run_pressed == true) {
      ball.setFillColor(sf::Color::Green);  // why isn't it green???
      ball.setOutlineColor(sf::Color::Green);

      positions.clear();
      setup = (getParametersFromUser(gui));
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
                                      system, setup, positions, speed, scale, gui)};

      gui.get<tgui::EditBox>("Final point")->setText(
          std::to_string(final_point.x) + "; " + std::to_string(final_point.y));
      i = 0;
      run_pressed = false;
    };

    if (i < positions.size() && positions.size() != 0) {
      ball.setPosition(positions[i].x, positions[i].y);
      i++;
    }

    if (i >= positions.size() && positions.size() != 0) {
      positions.clear();
      i = 0;
    }

    if (gaus_pressed == true) {
      setup = (getParametersFromUser(gui));
      getNormalDistribution(setup, gui);

      gaus_pressed = false;
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
GRANDE BUG !!!!!!! non funziona con linee divergenti perchè non trova
intersezione avanti ma solo indiettro perchè linee divergenti aumentano angolo
rispetto al lancio

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
