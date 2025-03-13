#ifndef FORWARDDECLARATION_H
#define FORWARDDECLARATION_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#include "TH1F.h"
#include "line.h"

struct Point {
  double x;
  double y;
};

struct Setup {
  double y_0;
  double theta_0;
  double l;
  double r_1;
  double r_2;
};

struct System {
  Line top_line;
  Line bottom_line;
  Line first_throw;
};

Setup getParametersFromUser();

Line makeLineFromPoints(Point p1, Point p2);

Line makeLineFromAngle(double theta, double q);

Point findInterception(Line l1, Line l2);

System makeSystemFromSetup(Setup s);

Point calculateFirstHit(Point interception_top_line,
                        Point interception_bottom_line, Setup setup,
                        System system);

Point getFinalPoint(Point new_interception, Point last_interception,
                    System system, Setup setup, std::vector<Point>& positions);

Point calculateFinalPoint(Point new_interception, Point last_interception,
                          System system, Setup setup, TH1F& h1, TH1F& h2);

void getNormalDistribution(Setup& setup);

void fillVector(std::vector<Point>& positions, Point last_interception,
                Point new_interception, Line path, double speed);

#endif
