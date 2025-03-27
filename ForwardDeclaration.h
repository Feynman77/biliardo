#ifndef FORWARDDECLARATION_H
#define FORWARDDECLARATION_H

#include <TCanvas.h>


#include <cmath>

#include <vector>
#include "graphic.h"

#include "line.h"

struct Point {
  double x;
  double y;
};

struct Setup {
  float y_0;
  float theta_0;
  float l;
  float r_1;
  float r_2;
};

struct System {
  Line top_line;
  Line bottom_line;
  Line first_throw;
};

Setup getParametersFromUser(const tgui::Gui &gui);
Line makeLineFromPoints(const Point &p_1, const Point &p_2);
Line makeLineFromAngle(const double &theta, const double &q);
Point findInterception(const Line &l_1, const Line &l_2);
System makeSystemFromSetup(const Setup &s);
Point calculateFirstHit(const double &l, const System &system);
void fillVector(std::vector<Point> &positions, const Point &last_interception,
                const Point &new_interception, const Line &path,
                const double &speed, const double &scale, const double &l);
void getFinalPoint(Point &new_interception, Point &last_interception,
                   const System &system, const double &l,
                   std::vector<Point> &positions, const double &speed,
                   const double &scale, tgui::Gui &gui);
void calculateFinalPoint(Point &new_interception, Point &last_interception,
                         const System &system, const double &l, TH1F &h1,
                         TH1F &h2);
void getNormalDistribution(const Setup &setup, tgui::Gui &gui);

#endif
