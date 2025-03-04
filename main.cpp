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
  double m{tan(theta * M_PI / 180)};
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

  std::cout << "Starting" << '\n';

  // ball hits top line
  if (interception_top_line.x < l && interception_top_line.x >= 0) {
    new_interception = interception_top_line;
    std::cout << "Ball hits top line" << '\n';
    std::cout << new_interception.x << '\n';
    std::cout << new_interception.y << '\n';
  };

  // ball hits bottom line
  if (interception_bottom_line.x < l && interception_bottom_line.x >= 0) {
    new_interception = interception_bottom_line;
    std::cout << "Ball hits bottom line" << '\n';
  };

  // ball goes directly out
  //  ci deve essere && al posto di || o sbaglio?
  if ((interception_top_line.x > l && interception_bottom_line.x < 0) ||
      (interception_bottom_line.x > l && interception_top_line.x < 0)) {
    double result{firstThrow.getSlope() * l + firstThrow.getQ()};
    std::cout << "The final coordinates are x=" << l << "and y=" << result
              << '\n';
    return 0;
  };

  std::cout << "First hit: done" << '\n';

  Line path(0, 0);

  // ball bounces (maybe)
  while (last_interception.x <= new_interception.x) {
    std::cout << "While loop" << '\n';

    // valid throw final path
    if (new_interception.x > l) {
      double result{path.getSlope() * l + path.getQ()};
      std::cout << "(rimbalzi) The final coordinates are x=" << l
                << " and y=" << result << '\n';
      return 0;
    };

    // ball hits top line first
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y > 0) {
      // we find the path after hitting the top line
      path.setSlope((l * firstThrow.getSlope() + r_2 - r_1) /
                    ((r_2 - r_1) * firstThrow.getSlope() - l));
      path.setQ(new_interception.y - path.getSlope() * new_interception.x);
      last_interception = new_interception;
      new_interception = findInterception(path, bottomLine);
      std::cout << "Top line: hit" << '\n';
    };

    // ball hits bottom line first
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y < 0) {
      // we find the path after hitting the bottom line
      path.setSlope(-1 * ((r_2 - r_1) * firstThrow.getSlope() - l) /
                    (l * firstThrow.getSlope() + r_2 - r_1));
      path.setQ(new_interception.y - path.getSlope() * new_interception.x);

      last_interception = new_interception;
      new_interception = findInterception(path, topLine);
      std::cout << "Bottom line: hit" << '\n';
    };

    // ball goes back
    if (new_interception.x < last_interception.x) {
      std::cout
          << "Invalid throw. The ball went behind the starting line. Try again."
          << '\n';
      return 0;
    };
  }
  std::cout
      << "Out of while (means no return 0 inside while activated. This is bad)"
      << '\n';
}

// Se togliessimo 7 gradi otterremo l'angolo giusto lol