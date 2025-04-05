#include "graphic.h"

#include "geometrical_methods.h"

// creation of a Tgui boxslider
auto createBoxSlider(const float& maximum, const float& minimum, const float& x,
                     const float& y) {
  auto boxSlider = tgui::EditBoxSlider::create(minimum, maximum);
  boxSlider->setPosition(x, y);
  boxSlider->setSize(200, 50);
  boxSlider->setValue((maximum - std::abs(minimum) / 2));
  boxSlider->setStep(0.01f);
  boxSlider->setDecimalPlaces(2);
  boxSlider->getSliderRenderer()->setThumbColor(tgui::Color::Cyan);
  return boxSlider;
}

// creation of a Tgui button
auto createButton(const float& x, const float& y, const char* name) {
  auto button = tgui::Button::create(name);
  button->setPosition(x, y);
  button->setSize(200, 50);
  button->setTextSize(15);
  button->getRenderer()->setBackgroundColor(tgui::Color::Green);
  return button;
}

// creation of a Tgui label
auto createLabel(const std::string& text, const float& x, const float& y) {
  auto label = tgui::Label::create(text);
  label->setPosition(x, y);
  label->setTextSize(15);
  label->getRenderer()->setTextColor(tgui::Color::White);
  return label;
}

// creation of a Tgui editbox
auto createEditBox(const std::string& text, const float& x, const float& y) {
  auto editbox = tgui::EditBox::create();
  editbox->setPosition(x, y);
  editbox->setSize(200, 50);
  editbox->setTextSize(18);
  editbox->setText(text);
  editbox->setReadOnly(true);
  return editbox;
}

// filling the TGui gui
void fillGui(tgui::Gui& gui) {
  // edit for motion
  gui.add(createBoxSlider(90, -90, 30, 550), "theta_0");
  gui.add(createBoxSlider(100, 0, 30, 650), "r_1");
  gui.add(createBoxSlider(100, 0, 280, 550), "y_0");
  gui.add(createBoxSlider(100, 0, 280, 650), "r_2");
  gui.add(createBoxSlider(1000, 0, 30, 750), "l");
  gui.add(createButton(280, 750, "Throw"), "run");

  // edit for normal distribution
  gui.add(createBoxSlider(10, 0, 30, 30), "sigma y_0");
  gui.add(createBoxSlider(40, 0, 280, 30), "sigma theta_0");
  gui.add(createBoxSlider(1E6, 0, 30, 105), "n");
  gui.add(createButton(280, 105, "Normal distribution"), "gauss");

  // adding sliders labels
  gui.add(createLabel("Theta_0", 30, 550 - 25), "label_theta_0");
  gui.add(createLabel("r_1", 30, 650 - 25), "label_r_1");
  gui.add(createLabel("y_0", 280, 550 - 25), "label_y_0");
  gui.add(createLabel("r_2", 280, 650 - 25), "label_r_2");
  gui.add(createLabel("l", 30, 750 - 25), "label_l");
  gui.add(createLabel("Sigma y_0", 30, 30 - 25), "label_sigma_y_0");
  gui.add(createLabel("Sigma theta_0", 280, 30 - 25), "label_sigma_theta_0");
  gui.add(createLabel("n", 30, 105 - 25), "label_n");

  // results of the throw
  gui.add(createEditBox("", 30.f, 850.f), "Final point");
  gui.add(createEditBox("", 280.f, 850.f), "Final angle");

  // results of the normal distribution
  gui.add(createEditBox("", 30.f, 180.f), "Position mean");
  gui.add(createEditBox("", 280.f, 180.f), "Angle mean");
  gui.add(createEditBox("", 30.f, 255.f), "Position stddev");
  gui.add(createEditBox("", 280.f, 255.f), "Angle stddev");
  gui.add(createEditBox("", 30.f, 330.f), "Position skewedness");
  gui.add(createEditBox("", 280.f, 330.f), "Angle skewedness");
  gui.add(createEditBox("", 30.f, 405.f), "Position kurtosis");
  gui.add(createEditBox("", 280.f, 405.f), "Angle kurtosis");

  // adding editbox labels
  gui.add(createLabel("Final position mean", 30, 180 - 25), "label_p_mean");
  gui.add(createLabel("Final angle mean", 280, 180 - 25), "label_a_mean");
  gui.add(createLabel("Final position std dev", 30, 255 - 25),
          "label_p_stddev");
  gui.add(createLabel("Final angle stddev", 280, 255 - 25), "label_a_stddev");
  gui.add(createLabel("Position Skewedness", 30, 330 - 25),
          "label_p_skewedness");
  gui.add(createLabel("Angle Skewedness", 280, 330 - 25), "label_a_skewedness");
  gui.add(createLabel("Position Kurtosis", 30, 405 - 25), "label_p_kurtosis");
  gui.add(createLabel("Angle Kurtosis", 280, 405 - 25), "label_a_kurtosis");
  gui.add(createLabel("Final position", 30, 850 - 25), "label_f_position");
  gui.add(createLabel("Angle of incidence", 280, 850 - 25), "label_f_angle");
}

// setting the layout for the opening window
void setWindow(sf::RenderWindow& window, sf::CircleShape& ball,
               sf::VertexArray& horizontal_line, sf::VertexArray& vertical_line,
               sf::Sprite& gauss_output_sprite) {
  ball.setFillColor(sf::Color::Black);
  ball.setOutlineColor(sf::Color::Black);

  // drawing the window
  sf::View view(sf::Vector2f(100, -225), sf::Vector2f(1400, 950));
  window.setView(view);
  window.setFramerateLimit(60);

  // centering and setting the window
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  float posX = (static_cast<float>(desktop.width) -
                static_cast<float>(window.getSize().x)) /
               2.f;
  float posY = (static_cast<float>(desktop.height) -
                static_cast<float>(window.getSize().y) - 100.f) /
               2.f;
  window.setPosition(
      sf::Vector2i(static_cast<int>(posX), static_cast<int>(posY)));

  // creation of horizontal line for layout
  horizontal_line[0].position = sf::Vector2f(-800, -228);
  horizontal_line[0].color = sf::Color::White;
  horizontal_line[1].position = sf::Vector2f(800, -228);
  horizontal_line[1].color = sf::Color::White;

  // creation of vertical line for layout
  vertical_line[0].position = sf::Vector2f(-50, -1000);
  vertical_line[0].color = sf::Color::White;
  vertical_line[1].position = sf::Vector2f(-50, 1000);
  vertical_line[1].color = sf::Color::White;

  // setting the position and dimension of the sprite
  gauss_output_sprite.setPosition(-50, -700);
  gauss_output_sprite.setScale(1.23f, 1.0f);
}