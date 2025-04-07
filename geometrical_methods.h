#ifndef geometrical_methods_H
#define geometrical_methods_H

#include <TCanvas.h>

#include <cmath>
#include <vector>

#include "geometrical_entities.h"

Point findInterception(const Line &l_1, const Line &l_2);

Point calculateFirstHit(const double &l, const System &system);

void fillVector(std::vector<Point> &positions, const Point &last_interception,
                const Point &new_interception, const Line &path,
                const Speed_and_scale &speed_and_scale, const double &l);

Angle_and_point getFinalPoint(Point &new_interception, Point &last_interception,
                              const System &system, const double &l,
                              std::vector<Point> &positions,
                              const Speed_and_scale &speed_and_scale);

void nThrowsSimulation(Point &new_interception, Point &last_interception,
                       const System &system, const double &l, TH1F &h1,
                       TH1F &h2);

void sliderUpdate(tgui::Gui &gui);
#endif
