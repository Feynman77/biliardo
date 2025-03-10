#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "line.h"
#include "ForwardDeclaration.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>

void openWindow(sf::RenderWindow& window, sf::CircleShape& ball, double l, double r_1, double r_2,
    double y_0, double speed, Line path, Point new_interception);


#endif