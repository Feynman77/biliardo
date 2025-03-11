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
                std::vector<Point> positions) {
  sf::View view(sf::Vector2f(0, 0), sf::Vector2f(800, 600));
  window.setView(view);
  sf::Clock clock;
  // sf::Window window(sf::VideoMode(400, 300), "My window");

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

  // run the program as long as the window is open
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    for (int i{0}; i <= positions.size(); i++) {
      ball.setPosition(positions[i].x, positions[i].y);
    }

    window.clear();
    window.draw(top_line);
    window.draw(bottom_line);
    window.draw(ball);
    window.display();
  }
}

/*void animateBottomHit(sf::RenderWindow& window, sf::CircleShape& ball,
                           double l, double r_1, double r_2, double y_0,
                           double speed, Line path, Point new_interception,
                           Point last_interception) {
  sf::View view(sf::Vector2f(0, 0), sf::Vector2f(800, 600));
  window.setView(view);
  sf::Clock clock;
  // sf::Window window(sf::VideoMode(400, 300), "My window");

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
  ball.setPosition(last_interception.x, last_interception.y);

  // run the program as long as the window is open
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    float deltaTime = clock.getElapsedTime().asSeconds();
    clock.restart();  // Time passed in seconds

    // Move the ball continuously
    if ((ball.getPosition().x <= new_interception.x * 25) &&
        (ball.getPosition().y >= new_interception.y * 25)) {
      // Move the ball based on the elapsed time to make movement smooth
      ball.move(speed * cos(atan(path.getSlope())) * deltaTime,
                speed * sin(atan(path.getSlope())) * deltaTime);
    }

    window.clear();
    window.draw(top_line);
    window.draw(bottom_line);
    window.draw(ball);
    window.display();
  }
}*/