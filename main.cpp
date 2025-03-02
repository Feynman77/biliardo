#include <cmath>
#include <iostream>
// #include "ball.h"

#include "line.h"
// #include "setting.h"
// #include "ForwardDeclaration.h"

struct Point {
  double x;
  double y;
};

Line makeLineFromPoints(Point p1, Point p2) {
  double m{(p2.y - p1.y) / (p2.x - p1.x)};
  double q{p1.y - m * p1.x};
  return Line(m, q);
}

Line makeLineFromAngle(double theta, double q) {
  double m{tan(theta * 3.141592653589793 / 180)};
  return Line(m, q);
}

Point findInterception(Line l1, Line l2) {
  if (l1.getSlope() != l2.getSlope()) {
    double abscissa{(l2.getQ() - l1.getQ()) / (l1.getSlope() - l2.getSlope())};
    double ordinate{l1.getSlope() * abscissa + l1.getQ()};
    Point p;
    p.x = abscissa;
    p.y = ordinate;
    return p;
  } else {
    Point p{-1, 0};  // x that gets discarded by main
    return p;
  }
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

  // Ball ball(theta_0, y_0);
  //  Setting setting(l, r_1, r_2);
  Line topLine(makeLineFromPoints({0, r_1}, {l, r_2}));
  Line bottomLine(makeLineFromPoints({0, -r_1}, {l, -r_2}));
  Line firstThrow(makeLineFromAngle(theta_0, y_0));
  Point last_interception{0, 0};
  Point interception_top_line{findInterception(topLine, firstThrow)};
  Point interception_bottom_line{findInterception(bottomLine, firstThrow)};
  Point new_interception{0, 0};

  std::cout << "1" << '\n';

  if (interception_top_line.x < l &&
      interception_top_line.x >= 0)  // ball hits top line
  {
    new_interception = interception_top_line;
  };
  if (interception_bottom_line.x < l &&
      interception_bottom_line.x >= 0)  // ball hits bottom line
  {
    new_interception = interception_bottom_line;
  };

  if ((interception_top_line.x > l && interception_bottom_line.x < 0) ||
      (interception_bottom_line.x > l ||
       interception_top_line.x < 0))  // ball goes directly out
  {
    double result{firstThrow.getSlope() * l + firstThrow.getQ()};
    std::cout << "The final coordinates are x=l and y=" << result << '\n';
    return 0;
  };

  std::cout << "12" << '\n';

  Line path(0, 0);

  while (last_interception.x <= new_interception.x) {
    std::cout << "While loop" << '\n';

    if (new_interception.x > l)  // valid throw final path
    {
      std::cout << "123" << '\n';
      double result{path.getSlope() * l + path.getQ()};
      std::cout << "(rimbalzi) The final coordinates are x=l and y=" << result
                << '\n';
      return 0;
    };

    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y > 0)  // ball hits top line
    {
      path.setSlope(l / (r_1 - r_2));
      path.setQ(
          new_interception.x * l / (r_2 - r_1) +
          new_interception.y);  // we find the path after hitting the top line
      last_interception = new_interception;
      new_interception = findInterception(path, bottomLine);
      std::cout << "top line: hit" << '\n';
    };

    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y < 0)  // ball hits bottom line
    {
      std::cout << "12345" << '\n';
      path.setSlope(l / (r_2 - r_1));
      path.setQ(new_interception.x * l / (r_1 - r_2) + new_interception.y);
      // we find the path after hitting the bottom line
      last_interception = new_interception;
      new_interception = findInterception(path, topLine);
    };

    if (new_interception.x < last_interception.x)  // ball goes back
    {
      std::cout
          << "Invalid throw. The ball went behind the starting line. Try again."
          << '\n';
      return 0;
    };
  }
  std::cout << "123456" << '\n';
}
