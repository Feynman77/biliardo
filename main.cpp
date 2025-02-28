#include <iostream>

#include "ball.h"
#include "line.h"
#include "setting.h"

struct Point {
  double x;
  double y;
};

Line makeLineFromPoints(Point p1, Point p2) {
  double m{(p2.y - p1.y) / (p2.x - p1.x)};
  double q{p1.y - m * p1.x};
  Line(m, q);
}

Line makeLineFromAngle(double theta, double q) {
  double m{tan(theta)};
  Line(m, q);
}

Point findInterception(Line l1, Line l2) {
  double abscissa{(l2.getQ() - l1.getQ()) / (l1.getSlope() - l2.getSlope())};
  double ordinate{l1.getSlope() * abscissa + l1.getQ()};
  Point p;
  p.x = abscissa;
  p.y = ordinate;
  return p;
}

int main() {
  double y_0{};
  double theta_0{};
  double l{};
  double r_1{};
  double r_2{};

  std::cout << "Insert y_0 and theta_0" << '\n';
  std::cin >> y_0 >> theta_0;
  std::cout << "Insert l, r_1 and r_2" << '\n';
  std::cin >> l >> r_1 >> r_2;

  Ball ball(theta_0, y_0);
  Setting setting(l, r_1, r_2);
  Point interception{0, 0};

  while () {
  }
  return 0;
}
