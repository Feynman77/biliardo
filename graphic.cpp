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
#include <TGUI/Color.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/EditBoxSlider.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include <vector>

#include "ForwardDeclaration.h"

// create a Tgui boxslider
auto createBoxSlider(double maximum, double minimum, double x, double y,
                     double step, double width, double height) {
  auto BoxSlider = tgui::EditBoxSlider::create(minimum, maximum);
  BoxSlider->setPosition(x, y);
  BoxSlider->setSize(width, height);
  BoxSlider->setValue((maximum - std::abs(minimum)) / 2);
  BoxSlider->setStep(step);
  BoxSlider->setDecimalPlaces(2);
  return BoxSlider;
}

// create a Tgui button
auto createButton(double x, double y, const char *name, double width,
                  double height) {
  auto Button = tgui::Button::create(name);
  Button->setPosition(x, y);
  Button->setSize(width, height);
  Button->setTextSize(15);
  Button->getRenderer()->setBackgroundColor(tgui::Color::Green);
  return Button;
}

// create a label
//  Function to create a label with the given text and position
auto createLabel(const std::string &text, float x, float y) {
  auto label = tgui::Label::create(text);
  label->getRenderer()->setTextColor(
      tgui::Color::White);  // Set text color to white
  label->setPosition(x, y); // Set the position
  label->setTextSize(15);
  return label;
}

auto createEditBox(const std::string &text, float x, float y) {

  auto editbox = tgui::EditBox::create();
  editbox->setPosition(x, y);
  editbox->setSize(200, 50);
  editbox->setTextSize(18);
  editbox->setText(text);
  editbox->setReadOnly(true);
  return editbox;
}

// fill the TGui gui
void fillGui(tgui::Gui &gui) {
  // edit for motion
  gui.add(createBoxSlider(90, -90, 30, 550, 0.01, 200, 50), "theta_0");
  gui.add(createBoxSlider(100, 0, 30, 650, 0.01, 200, 50), "r_1");

  float r_1 = gui.get<tgui::EditBoxSlider>("r_1")->getValue();

  gui.add(createBoxSlider(r_1, -r_1, 280, 550, 0.01, 200, 50), "y_0");
  gui.add(createBoxSlider(100, 0, 280, 650, 0.01, 200, 50), "r_2");
  gui.add(createBoxSlider(1000, 0, 30, 750, 0.01, 200, 50), "l");
  gui.add(createButton(280, 750, "Throw", 200, 50), "run");

  // edit for gaussian distribution
  gui.add(createBoxSlider(10, 0, 30, 30, 0.01, 200, 50), "sigma y_0");
  gui.add(createBoxSlider(40, 0, 280, 30, 0.01, 200, 50), "sigma theta_0");
  gui.add(createBoxSlider(10000, 0, 30, 105, 0.01, 200, 50), "n");
  gui.add(createButton(280, 105, "Normal distribution", 200, 50), "gauss");

  // Add labels for sliders
  gui.add(createLabel("theta_0", 30, 550 - 25), "label_theta_0");
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
//results of the normal distribution
  gui.add(createEditBox("", 30.f, 180.f), "Position mean");
  gui.add(createEditBox("", 280.f, 180.f), "Angle mean");
  gui.add(createEditBox("", 30.f, 255.f), "Position stddev");
  gui.add(createEditBox("", 280.f, 255.f), "Angle stddev");
  gui.add(createEditBox("", 30.f, 330.f), "Position skewedness");
  gui.add(createEditBox("", 280.f, 330.f), "Angle skewedness");
  gui.add(createEditBox("", 30.f, 405.f), "Position kurtosis");
  gui.add(createEditBox("", 280.f, 405.f), "Angle kurtosis");

  //editbox labels

  gui.add(createLabel("Final position mean", 30, 180 - 25), "label_p_mean");
  gui.add(createLabel("Final angle mean", 280, 180 - 25), "label_a_mean");
  gui.add(createLabel("Final position std dev", 30, 255 - 25), "label_p_stddev");
  gui.add(createLabel("Final angle stddev", 280, 255 - 25), "label_a_stddev");
  gui.add(createLabel("Position Skewedness", 30, 330 - 25), "label_p_skewedness");
  gui.add(createLabel("Angle Skewedness", 280, 330 - 25), "label_a_skewedness");
  gui.add(createLabel("Position Kurtosis", 30, 405 - 25), "label_p_kurtosis");
  gui.add(createLabel("Angle Kurtosis", 280, 405 - 25), "label_a_kurtosis");
  gui.add(createLabel("Final position", 30, 850 - 25), "label_f_position");
  gui.add(createLabel("Angle of incidence", 280, 850 - 25), "label_f_angle");
}

void makeDrawableSystem(sf::CircleShape &ball, sf::VertexArray &top_line,
                        sf::VertexArray &bottom_line, Setup &s, double& scale) {
  // creation of top line
  top_line[0].position = sf::Vector2f(0, scale * s.r_1);
  top_line[0].color = sf::Color::White;
  top_line[1].position = sf::Vector2f(scale * s.l, scale * s.r_2);
  top_line[1].color = sf::Color::White;

  // creation of the bottom line
  bottom_line[0].position = sf::Vector2f(0, -scale * s.r_1);
  bottom_line[0].color = sf::Color::White;
  bottom_line[1].position = sf::Vector2f(scale * s.l, -scale * s.r_2);
  bottom_line[1].color = sf::Color::White;

  // creation of the ball
  ball.setFillColor(sf::Color::White);
  ball.setPosition(0, scale * s.y_0);
  ball.setOrigin(ball.getRadius(), ball.getRadius());
}