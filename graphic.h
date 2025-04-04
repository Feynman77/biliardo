#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Color.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/EditBoxSlider.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Picture.hpp>
//struct Setup;

void setWindow(sf::RenderWindow& window, sf::CircleShape& ball,
               sf::VertexArray& hor_line, sf::VertexArray& vert_line,
               sf::Sprite& sprite);

auto createBoxSlider(const float& maximum, const float& minimum, const float& x,
                     const float& y);

auto createButton(const float& x, const float& y, const char* name);

auto createLabel(const std::string& text, const float& x, const float& y);

auto createEditBox(const std::string& text, const float& x, const float& y);

void fillGui(tgui::Gui& gui);



#endif