#include <iostream>

#include "ball.h"
#include "line.h"
#include "setting.h"

struct point {
  double x;
  double y;
};

line makeLineFromPoints(point p1, point p2) {
  double m{(p2.y - p1.y) / (p2.x - p1.x)};
  double q{p1.y - m * p1.x};
  line(m, q);
}

line makeLineFromAngle(double theta, double q) {
  double m{tan(theta)};
  line(m, q);
}

point findInterception(line l1, line l2) {
  double abscissa{(l2.getQ() - l1.getQ()) / (l1.getSlope() - l2.getSlope())};
  double ordinate{l1.getSlope() * abscissa + l1.getQ()};
  point p;
  p.x = abscissa;
  p.y = ordinate;
 return p;
}

int main() { return 0; }
