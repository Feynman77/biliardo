#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Color.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/EditBoxSlider.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Picture.hpp>


#include "cmath"
#include "doctest.h"

struct Line {
  double slope;
  double q;
};
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

struct Angle_and_point {
  double theta;
  double y;
};

// calculate m and q of a line from two point
// forse metodo di classe (costruttore in overload?)
Line makeLineFromPoints(const Point &p_1, const Point &p_2) {
  double slope{(p_2.y - p_1.y) / (p_2.x - p_1.x)};
  double q{p_1.y - slope * p_1.x};
  return Line{slope, q};
}

// calculate of m and q of a line from an angle
// idem sopra
Line makeLineFromAngle(const double &theta, const double &q) {
  double slope{tan(theta * M_PI / 180)};
  return Line{slope, q};
}

// find the interception beetween two lines
Point findInterception(const Line &l_1, const Line &l_2) {
  if (l_1.slope != l_2.slope) {
    double abscissa{(l_2.q - l_1.q) / (l_1.slope - l_2.slope)};
    double ordinate{l_1.slope * abscissa + l_1.q};
    Point p;
    p.x = abscissa;
    p.y = ordinate;
    return p;
  } else {
    Point p{-1, 0};  // x that gets discarded by main
    return p;
  }
}

// make system from setup
System makeSystemFromSetup(const Setup &s) {
  Line top_line(makeLineFromPoints({0, s.r_1}, {s.l, s.r_2}));
  Line bottom_line(makeLineFromPoints({0, -s.r_1}, {s.l, -s.r_2}));
  Line first_throw(makeLineFromAngle(s.theta_0, s.y_0));
  System system{top_line, bottom_line, first_throw};
  return system;
}

// calculate the first hit
Point calculateFirstHit(const double &l, const System &system) {
  Point new_interception;
  Point interception_top_line =
      findInterception(system.top_line, system.first_throw);
  Point interception_bottom_line =
      findInterception(system.bottom_line, system.first_throw);

  // ball hits top line
  if (interception_top_line.x >= 0 && interception_top_line.x <= l) {
    new_interception = interception_top_line;
  };

  // ball hits bottom line
  if (interception_bottom_line.x >= 0 && interception_bottom_line.x <= l) {
    new_interception = interception_bottom_line;
  };

  // ball goes directly out
  if ((interception_top_line.x > l && interception_bottom_line.x < 0) ||
      (interception_bottom_line.x > l && interception_top_line.x < 0) ||
      (interception_top_line.x > l && interception_bottom_line.x > l) ||
      (interception_top_line.x < 0 && interception_bottom_line.x < 0)) {
    double result{system.first_throw.slope * l + system.first_throw.q};
    new_interception.x = l;
    new_interception.y = result;
  };

  return new_interception;
}

// fill the vector with the positions of the motion
void fillVector(std::vector<Point> &positions, const Point &last_interception,
                const Point &new_interception, const Line &path,
                const double &speed, const double &scale, const double &l) {
  int i{0};
  if (last_interception.x < new_interception.x) {
    do {
      double x = last_interception.x + i * (speed / 30) * cos(atan(path.slope));
      Point position{scale * x, scale * path.slope * x + scale * path.q};

      positions.emplace_back(position);
      i++;
    } while ((positions.back().x <= scale * new_interception.x) &&
             (positions.back().x <= scale * l));
  } else {
    do {
      double x = last_interception.x - i * (speed / 30) * cos(atan(path.slope));
      Point position{scale * x, scale * path.slope * x + scale * path.q};

      positions.emplace_back(position);
      i++;
    } while ((positions.back().x >= scale * new_interception.x) &&
             (positions.back().x >= 0));
  }
}

// calculate the final point
Angle_and_point getFinalPoint(Point &new_interception, Point &last_interception,
                              const System &system, const double &l,
                              std::vector<Point> &positions,
                              const double &speed, const double &scale) {
  // setting the initial situation
  Line path{system.first_throw.slope, system.first_throw.q};
  Angle_and_point p;

  // ball start bouncing
  while (last_interception.x <= new_interception.x) {
    // valid throw final path (give us the result)
    if (new_interception.x >= l) {
      fillVector(positions, last_interception, new_interception, path, speed,
                 scale, l);

      // calculate the final informations
      double result{path.slope * l +
                    path.q};  // wrong sign but there is a reason ( we do
                              // the calculations on the flipped system
                              // because sfml is flipped)
      double final_angle{atan(path.slope)};

      p.theta = -final_angle * 180 / M_PI;
      p.y = -result;
      break;
    };

    // ball hits top line first
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y > 0) {
      fillVector(positions, last_interception, new_interception, path, speed,
                 scale, l);

      // path after the ball hit the top line
      path.slope =
          ((path.slope * system.top_line.slope * system.top_line.slope -
            path.slope + 2 * system.top_line.slope) /
           (1 - system.top_line.slope * system.top_line.slope +
            2 * path.slope * system.top_line.slope));
      path.q = (new_interception.y - path.slope * new_interception.x);

      last_interception = new_interception;
      new_interception = findInterception(path, system.bottom_line);
    };

    // ball hits bottom line first
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y < 0) {
      fillVector(positions, last_interception, new_interception, path, speed,
                 scale, l);

      // path after the ball hit the bottom line
      path.slope = (-path.slope);
      path.slope =
          ((-1) *
           (path.slope * system.top_line.slope * system.top_line.slope -
            path.slope + 2 * system.top_line.slope) /
           (1 - system.top_line.slope * system.top_line.slope +
            2 * path.slope * system.top_line.slope));
      path.q = (new_interception.y - path.slope * new_interception.x);

      last_interception = new_interception;
      new_interception = findInterception(path, system.top_line);
    };

    // divergent lines
    if ((system.top_line.slope > system.bottom_line.slope) &&
        (new_interception.x < 0)) {
      new_interception.y = path.slope * l * path.q;
      new_interception.x = l;
      fillVector(positions, last_interception, new_interception, path, speed,
                 scale, l);

      double result{path.slope * l + path.q};  // wrong sign for a reason
      double final_angle{atan(path.slope)};

      p.theta = -final_angle * 180 / M_PI;
      p.y = -result;

      break;
    };

    // ball goes back
    if (new_interception.x < last_interception.x) {
      while (last_interception.x >= new_interception.x &&
             last_interception.x > 0) {
        // ball hits top line first (Francesco)
        if (new_interception.x < l && new_interception.y > 0 &&
            last_interception.x >= 0) {
          // we find the path after hitting the top line
          fillVector(positions, last_interception, new_interception, path,
                     speed, scale, l);
          path.slope =
              ((path.slope * system.top_line.slope * system.top_line.slope -
                path.slope + 2 * system.top_line.slope) /
               (1 - system.top_line.slope * system.top_line.slope +
                2 * path.slope * system.top_line.slope));
          path.q = (new_interception.y - path.slope * new_interception.x);

          last_interception = new_interception;
          new_interception = findInterception(path, system.bottom_line);
        };

        // ball hits bottom line first (Francesco)
        if (new_interception.x < l && new_interception.y < 0 &&
            last_interception.x >= 0) {
          fillVector(positions, last_interception, new_interception, path,
                     speed, scale, l);
          // we find the path after hitting the bottom line
          path.slope = (-path.slope);
          path.slope =
              ((-1) *
               (path.slope * system.top_line.slope * system.top_line.slope -
                path.slope + 2 * system.top_line.slope) /
               (1 - system.top_line.slope * system.top_line.slope +
                2 * path.slope * system.top_line.slope));
          path.q = (new_interception.y - path.slope * new_interception.x);

          last_interception = new_interception;
          new_interception = findInterception(path, system.top_line);
        };
      };
      p.theta = 180;
      p.y = 0;
    };
  };

  return p;
}

TEST_CASE("Testing getfinalpoint") {
  SUBCASE("Straight throw") {
    Setup setup = {0, 0, 10, 4, 4};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(10, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    CHECK(getFinalPoint(new_interception, last_interception,
                        makeSystemFromSetup(setup), 10, positions, 25, 25)
              .y == doctest::Approx(0));
    CHECK(getFinalPoint(new_interception, last_interception,
                        makeSystemFromSetup(setup), 10, positions, 25, 25)
              .theta == doctest::Approx(0));
  }
  SUBCASE("40 deg throw convergent lines") {
    Setup setup = {0, -40, 10, 4, 3};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(10, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception,
                      makeSystemFromSetup(setup), 10, positions, 25, 25);
    CHECK(result.y == doctest::Approx(-2.149506));
    CHECK(result.theta == doctest::Approx(62.84237));
  }
  SUBCASE("40 deg divergent lines") {
    Setup setup = {0, -40, 10, 4, 6};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(10, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception,
                      makeSystemFromSetup(setup), 10, positions, 25, 25);
    CHECK(result.y == doctest::Approx(4.080766));
    CHECK(result.theta == doctest::Approx(-17.38014));
  }
  SUBCASE("Divergent line, less steep path than top line") {
    Setup setup = {0, -75, 18, 4, 8};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(18, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception,
                      makeSystemFromSetup(setup), 18, positions, 25, 25);
    CHECK(result.y == doctest::Approx(-2.467113));
    CHECK(result.theta == doctest::Approx(24.884769));
  }
  SUBCASE("divergent lines almost extreme case") {
    Setup setup = {-2, -80, 15, 3, 8};
    Point last_interception{0, -2};
    Point new_interception{calculateFirstHit(15, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception,
                      makeSystemFromSetup(setup), 15, positions, 25, 25);
    CHECK(result.y == doctest::Approx(-5.934306));
    CHECK(result.theta == doctest::Approx(6.260205));
  }
  SUBCASE("parallel lines") {
    Setup setup = {0, -45, 40, 5, 5};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(40, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception,
                      makeSystemFromSetup(setup), 40, positions, 25, 25);
    CHECK(result.y == doctest::Approx(0));
    CHECK(result.theta == doctest::Approx(45));
  }
  SUBCASE("40 deg throw convergent lines") {
    Setup setup = {0, 40, 10, 4, 3};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(10, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception,
                      makeSystemFromSetup(setup), 10, positions, 25, 25);
    CHECK(result.y == doctest::Approx(2.149506));
    CHECK(result.theta == doctest::Approx(-62.84237));
  }
  SUBCASE("40 deg divergent lines") {
    Setup setup = {0, 40, 10, 4, 6};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(10, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception,
                      makeSystemFromSetup(setup), 10, positions, 25, 25);
    CHECK(result.y == doctest::Approx(-4.080766));
    CHECK(result.theta == doctest::Approx(17.38014));
  }
  SUBCASE("Divergent line, less steep path than top line") {
    Setup setup = {0, 75, 18, 4, 8};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(18, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception,
                      makeSystemFromSetup(setup), 18, positions, 25, 25);
    CHECK(result.y == doctest::Approx (2.467113));
    CHECK(result.theta == doctest::Approx(-24.884769));
  }
  SUBCASE("divergent lines almost extreme case") {
    Setup setup = {2, 80, 15, 3, 8};
    Point last_interception{0, 2};
    Point new_interception{calculateFirstHit(15, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception,
                      makeSystemFromSetup(setup), 15, positions, 25, 25);
    CHECK(result.y == doctest::Approx(5.934306));
    CHECK(result.theta == doctest::Approx(-6.260205));
  }
  SUBCASE("parallel lines") {
    Setup setup = {0, 45, 40, 5, 5};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(40, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception,
                      makeSystemFromSetup(setup), 40, positions, 25, 25);
    CHECK(result.y == doctest::Approx(0));
    CHECK(result.theta == doctest::Approx(-45));
  }
}