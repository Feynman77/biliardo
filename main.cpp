
#include <SFML/System/Clock.hpp>
#include <string>

#include "geometrical_entities.h"
#include "geometrical_methods.h"
#include "graphic.h"

int main() {
  // initial setting
  sf::RenderWindow window{sf::VideoMode(1400, 950), "Biliardo",
                          sf::Style::Titlebar | sf::Style::Close};
  tgui::Gui gui{window};
  fillGui(gui);

  Speed_and_scale speed_and_scale{25, 2};

  bool throw_pressed{false};
  bool normal_distribution_pressed{false};
  unsigned int frame_index{0};

  std::vector<Point> animation_positions{};

  // layout
  sf::CircleShape ball(1);
  sf::VertexArray top_line(sf::Lines, 2);
  sf::VertexArray bottom_line(sf::Lines, 2);
  sf::VertexArray horizontal_line(sf::Lines, 2);
  sf::VertexArray vertical_line(sf::Lines, 2);

  // images of the gaussian distribution
  sf::Texture gauss_output_texture;
  sf::Sprite gauss_output_sprite;

  setWindow(window, ball, horizontal_line, vertical_line, gauss_output_sprite);

  gui.get<tgui::Button>("gauss")->onClick(
      [&]() { normal_distribution_pressed = true; });
  gui.get<tgui::Button>("run")->onClick([&]() { throw_pressed = true; });

  // while loop for the open window
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
      // deny resizing
      if (event.type == sf::Event::Resized) {
        window.setSize(sf::Vector2u(1400, 950));
      }
    }

    // updating in runtime the possible values for the y_0 slider
    sliderUpdate(gui);

    // press the button "Throw"
    if (throw_pressed == true) {
      Setup setup(gui);
      setup.run(ball, animation_positions, speed_and_scale, gui, top_line,
                bottom_line);

      // set frame_index and throw_pressed so that after a click it restarts
      frame_index = 0;
      throw_pressed = false;
    };

    // press the button "Run"
    if (normal_distribution_pressed == true) {
      Setup setup(gui);
      setup.getNormalDistribution(gui);

      gauss_output_texture.loadFromFile("histos.png");
      gauss_output_sprite.setTexture(gauss_output_texture);

      normal_distribution_pressed = false;
    };

    // animation of the ball
    if (frame_index < animation_positions.size() &&
        !animation_positions.empty()) {
      ball.setPosition(static_cast<float>(animation_positions[frame_index].x),
                       static_cast<float>(animation_positions[frame_index].y));
      frame_index++;
    }

    // refreshing the ball position
    if (frame_index >= animation_positions.size() &&
        !animation_positions.empty()) {
      animation_positions.clear();
      frame_index = 0;
    }

    // drawing the filled window
    window.clear();
    window.draw(horizontal_line);
    window.draw(vertical_line);
    window.draw(top_line);
    window.draw(bottom_line);
    window.draw(ball);
    window.draw(gauss_output_sprite);
    gui.draw();
    window.display();
  }
  return 0;
}
