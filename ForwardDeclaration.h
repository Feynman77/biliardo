#ifndef FORWARDDECLARATION_H
#define FORWARDDECLARATION_H

#include <TCanvas.h>

#include <cmath>
#include <vector>

#include "structs.h"

// struct Line {
//   double slope;
//   double q;
// };
// struct Point {
//   double x;
//   double y;
// };

// struct Speed_and_scale {
//   float speed;
//   float scale;
// };

// // struct Setup {
// //   float y_0;
// //   float theta_0;
// //   float l;
// //   float r_1;
// //   float r_2;
// // };

// struct System {
//   Line top_line;
//   Line bottom_line;
//   Line first_throw;
// };

// struct Angle_and_point {
//   double theta;
//   double y;
// };

// //Setup getParametersFromUser(const tgui::Gui &gui);

// Line makeLineFromPoints(const Point &p_1, const Point &p_2);

// Line makeLineFromAngle(const double &theta, const double &q);

Point findInterception(const Line &l_1, const Line &l_2);

Point calculateFirstHit(const double &l, const System &system);

Angle_and_point getFinalPoint(Point &new_interception, Point &last_interception,
                              const System &system, const double &l,
                              std::vector<Point> &positions,
                              const Speed_and_scale &speed_and_scale);

void calculateFinalPoint(Point &new_interception, Point &last_interception,
                         const System &system, const double &l, TH1F &h1,
                         TH1F &h2);

void fillVector(std::vector<Point> &positions, const Point &last_interception,
                const Point &new_interception, const Line &path,
                const Speed_and_scale &speed_and_scale, const double &l);

// void getNormalDistribution(const Setup &setup, tgui::Gui &gui);

// void run(sf::CircleShape &ball, std::vector<Point> &positions,
//          Speed_and_scale &speed_and_scale, tgui::Gui &gui,
//          sf::VertexArray &top_line, sf::VertexArray &bottom_line);

void sliderUpdate(tgui::Gui &gui);
#endif
