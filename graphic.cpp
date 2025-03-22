#include "graphic.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <vector>

#include "ForwardDeclaration.h"

void openWindow(sf::RenderWindow& window, sf::CircleShape& ball, double l,
                double r_1, double r_2, double y_0,
                std::vector<Point> positions, tgui::Gui& gui) {
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

  // cration of the top line
  sf::VertexArray top_line(sf::Lines, 2);
  top_line[0].position = sf::Vector2f(0, r_1);
  top_line[0].color = sf::Color::White;

  top_line[1].position = sf::Vector2f(l, r_2);
  top_line[1].color = sf::Color::White;

  // creation of the bottom line
  sf::VertexArray bottom_line(sf::Lines, 2);

  bottom_line[0].position = sf::Vector2f(0, -r_1);
  bottom_line[0].color = sf::Color::White;

  bottom_line[1].position = sf::Vector2f(l, -r_2);
  bottom_line[1].color = sf::Color::White;

  // creation of the ball
  ball.setFillColor(sf::Color::White);
  ball.setPosition(0, y_0);
  ball.setOrigin(ball.getRadius(), ball.getRadius());

  int i{0};
  // run the program as long as the window is open
  while (window.isOpen()) {
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

auto createBoxSlider(double maximum, double minimum, double x,
                  double y, double step, double width, double height) {
                    auto BoxSlider= tgui::EditBoxSlider::create(minimum, maximum);
                    BoxSlider->setPosition(x,y);
                    BoxSlider->setSize(width, height);
                    BoxSlider->setValue((maximum-minimum)/2);
                    BoxSlider->setStep(step);
                    return BoxSlider;
                  }

void fillGui(tgui::Gui& gui) {
  gui.add(createBoxSlider(10, 0, 0, 20, 1, 200, 20));
  gui.add(createBoxSlider(10, 0, 250, 20, 1, 200, 20));
  gui.add(createBoxSlider(10, 0, 0, 45, 1, 200, 20));
  gui.add(createBoxSlider(10, 0, 250, 45, 1, 200, 20));
  gui.add(createBoxSlider(10, 0, 0, 70, 1, 200, 20));
}
