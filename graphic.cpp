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
#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/EditBoxSlider.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <vector>

#include "ForwardDeclaration.h"

// creation of the window
/*void openWindow(sf::RenderWindow& window, sf::CircleShape& ball, double l,
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
}*/

// create a Tgui boxslider
auto createBoxSlider(double maximum, double minimum, double x, double y,
                     double step, double width, double height) {
  auto BoxSlider = tgui::EditBoxSlider::create(minimum, maximum);
  BoxSlider->setPosition(x, y);
  BoxSlider->setSize(width, height);
  BoxSlider->setValue((maximum - minimum) / 2);
  BoxSlider->setStep(step);
  return BoxSlider;
}

// create a Tgui button
auto createButton(double x, double y, const char* name, double width,
                  double height) {
  auto Button = tgui::Button::create(name);
  Button->setPosition(x, y);
  Button->setSize(width, height);
  return Button;
}

// fill the TGui gui
void fillGui(tgui::Gui& gui) {
  gui.add(createBoxSlider(90, -90, 0, 20, 0.01, 200, 20), "theta_0");
  gui.add(createBoxSlider(10, 0, 0, 45, 0.01, 200, 20), "r_1");

  float r_1 = gui.get<tgui::EditBoxSlider>("r_1")->getValue();

  gui.add(createBoxSlider(r_1, -r_1, 250, 20, 0.01, 200, 20), "y_0");
  gui.add(createBoxSlider(10, 0, 250, 45, 0.01, 200, 20), "r_2");
  gui.add(createBoxSlider(10, 0, 0, 70, 0.01, 200, 20), "l");
  gui.add(createButton(250, 70, "run", 200, 20), "run");
}

// NEW OPENWINDOW
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

void makeDrawableSystem(sf::CircleShape& ball, sf::VertexArray& top_line,
                        sf::VertexArray& bottom_line, Setup& s) {
  top_line[0].position = sf::Vector2f(0, 25 * s.r_1);
  top_line[0].color = sf::Color::White;
  top_line[1].position = sf::Vector2f(25 * s.l, 25 * s.r_2);
  top_line[1].color = sf::Color::White;

  // creation of the bottom line

  bottom_line[0].position = sf::Vector2f(0, -25 * s.r_1);
  bottom_line[0].color = sf::Color::White;
  bottom_line[1].position = sf::Vector2f(25 * s.l, -25 * s.r_2);
  bottom_line[1].color = sf::Color::White;

  // creation of the ball
  ball.setFillColor(sf::Color::White);
  ball.setPosition(0, 25 * s.y_0);
  ball.setOrigin(ball.getRadius(), ball.getRadius());
}