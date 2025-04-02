
#include <SFML/System/Clock.hpp>
#include <string>

#include "ForwardDeclaration.h"

int main() {
  // initial setting
  sf::RenderWindow window{sf::VideoMode(1400, 950), "Biliardo",
                          sf::Style::Titlebar | sf::Style::Close};
  tgui::Gui gui{window};
  fillGui(gui);

  Setup setup;

  // TODO uniformare la sintassi di inizializzazione (= vs {})
  bool throw_pressed = false;
  bool normal_distribution_pressed = false;

  float scale{25};
  float speed{2};

  // FIXME brutto nome
  unsigned int i{0};

  std::vector<Point> animation_positions{};

  // interfaccia
  sf::CircleShape ball(1);
  sf::VertexArray top_line(sf::Lines, 2);
  sf::VertexArray bottom_line(sf::Lines, 2);
  sf::VertexArray hor_line(sf::Lines, 2);
  sf::VertexArray vert_line(sf::Lines, 2);

  // FIXME Dammi un nome più descrittivo
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
    while (window.pollEvent(event)) {
      gui.handleEvent(event);
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      //deny resizing
      if (event.type == sf::Event::Resized) {
        window.setSize(sf::Vector2u(1400, 950));
      }
    }

    sliderUpdate(gui);

    if (throw_pressed == true) {
      run(ball, animation_positions, setup, scale, speed, gui, top_line, bottom_line);
      i = 0;
      throw_pressed = false;
    };

    if (i < animation_positions.size() && !animation_positions.empty()) {
      ball.setPosition(static_cast<float>(animation_positions[i].x),
                       static_cast<float>(animation_positions[i].y));
      i++;
    }

    if (i >= animation_positions.size() && !animation_positions.empty()) {
      animation_positions.clear();
      i = 0;
    }

    if (normal_distribution_pressed == true) {
      normal_distribution_pressed = false;
      
      setup = getParametersFromUser(gui);
      getNormalDistribution(setup, gui);

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
