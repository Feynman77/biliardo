#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <vector>

#include "ForwardDeclaration.h"
#include "line.h"

void animateBottomHit(sf::RenderWindow &window, sf::CircleShape &ball, double l,
                      double r_1, double r_2, double y_0, double speed,
                      Line path, Point new_interception,
                      Point last_interception);

auto createBoxSlider(double maximum, double minimum, double x, double y,
                     double step, double width, double height);

auto createButton(double x, double y, const char *name, double width,
                  double height);

auto createLabel(const std::string &text, float x, float y);

auto createEditBox(std::string &text, float x, float y);

void makeDrawableSystem(sf::CircleShape &ball, sf::VertexArray &top_line,
                        sf::VertexArray &bottom_line, Setup &s);

void fillGui(tgui::Gui &gui);

#endif