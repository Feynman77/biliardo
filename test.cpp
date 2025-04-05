#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "cmath"
#include "doctest.h"
#include "geometrical_entities.h"
#include "geometrical_methods.h"
#include "graphic.h"

// The results of calculateFirstHit and getFinalPoint are the opposite of what
// is expected because the input values from the setup are flipped by the
// constructor (y_0, theta_0), so the function performs calculations using the
// opposites.

Speed_and_scale speed_and_scale{25, 25};

TEST_CASE("Test findInterception") {
  SUBCASE("Upwards line with downwards line") {
    Line l_1{Line({0, 4}, {7, 5})};
    Angle_and_point a_p{45, 3};
    Line l_2{Line(a_p)};
    Point result = findInterception(l_1, l_2);
    CHECK(result.x == doctest::Approx(1.167).epsilon(0.001));
    CHECK(result.y == doctest::Approx(4.167).epsilon(0.001));
  }
  SUBCASE("Downwards line with upwards line") {
    Line l_1{Line({0, -2}, {10, -1})};
    Angle_and_point a_p{-45, -1};
    Line l_2{Line(a_p)};
    Point result = findInterception(l_1, l_2);
    CHECK(result.x == doctest::Approx(0.91).epsilon(0.001));
    CHECK(result.y == doctest::Approx(-1.91).epsilon(0.001));
  }

  SUBCASE("Parallel lines") {
    Line l_1{Line({4, 4}, {7, 4})};
    Line l_2{Line(0, 0)};
    Point result = findInterception(l_1, l_2);
    CHECK(result.x == doctest::Approx(-1).epsilon(0.001));
    CHECK(result.y == doctest::Approx(0).epsilon(0.001));
  }
}

TEST_CASE("Test calculateFirstHit") {
  SUBCASE("No line") {
    double l = 4;
    Setup s(-3, 23.2f, 4, 4, 2);
    System system(s);
    Point result = calculateFirstHit(l, system);
    CHECK(result.x == doctest::Approx(4).epsilon(0.01));
    CHECK(result.y == doctest::Approx(1.29).epsilon(0.01));
  }
  SUBCASE("Hit bottom  line (drawn)") {
    double l = 6;
    Setup s(1, 56.31f, 6, 4, 3);
    System system(s);
    Point result = calculateFirstHit(l, system);
    CHECK(result.x == doctest::Approx(1.8).epsilon(0.01));
    CHECK(result.y == doctest::Approx(-3.7).epsilon(0.01));
  }
  SUBCASE("Hit top line (drawn)") {
    double l = 10;
    Setup s(-1, -20.03f, 10, 4, 2.5);
    System system(s);
    Point result = calculateFirstHit(l, system);
    CHECK(result.x == doctest::Approx(5.83).epsilon(0.01));
    CHECK(result.y == doctest::Approx(3.13).epsilon(0.01));
  }
}

TEST_CASE("Testing getFinalPoint") {
  SUBCASE("Straight throw") {
    Setup setup = {0, 0, 10, 4, 4};
    System system(setup);
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(10, system)};
    std::vector<Point> positions;
    CHECK(getFinalPoint(new_interception, last_interception, system, 10,
                        positions, speed_and_scale)
              .y == doctest::Approx(0));
    CHECK(getFinalPoint(new_interception, last_interception, system, 10,
                        positions, speed_and_scale)
              .theta == doctest::Approx(0));
  }
  SUBCASE("40 degree throw convergent lines") {
    Setup setup = {0, -40, 10, 4, 3};
    System system(setup);
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(10, system)};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception, system, 10,
                      positions, speed_and_scale);
    CHECK(result.y == doctest::Approx(2.149506));
    CHECK(result.theta == doctest::Approx(-62.84237));
  }
  SUBCASE("40 degree divergent lines") {
    Setup setup = {0, -40, 10, 4, 6};
    System system(setup);
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(10, system)};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception, system, 10,
                      positions, speed_and_scale);
    CHECK(result.y == doctest::Approx(-4.080766));
    CHECK(result.theta == doctest::Approx(17.38014));
  }
  SUBCASE("Divergent line, less steep path than top line") {
    Setup setup = {0, -75, 18, 4, 8};
    System system(setup);
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(18, system)};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception, system, 18,
                      positions, speed_and_scale);
    CHECK(result.y == doctest::Approx(2.467113));
    CHECK(result.theta == doctest::Approx(-24.884769));
  }
  SUBCASE("Divergent lines almost extreme case") {
    Setup setup = {-2, -80, 15, 3, 8};
    System system(setup);
    Point last_interception{0, -2};
    Point new_interception{calculateFirstHit(15, system)};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception, system, 15,
                      positions, speed_and_scale);
    CHECK(result.y == doctest::Approx(5.934306));
    CHECK(result.theta == doctest::Approx(-6.260205));
  }
  SUBCASE("Parallel lines") {
    Setup setup = {0, -45, 40, 5, 5};
    System system(setup);
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(40, system)};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception, system, 40,
                      positions, speed_and_scale);
    CHECK(result.y == doctest::Approx(0));
    CHECK(result.theta == doctest::Approx(-45));
  }
  SUBCASE("40 degree throw convergent lines") {
    Setup setup = {0, 40, 10, 4, 3};
    System system(setup);
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(10, system)};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception, system, 10,
                      positions, speed_and_scale);
    CHECK(result.y == doctest::Approx(-2.149506));
    CHECK(result.theta == doctest::Approx(62.84237));
  }
  SUBCASE("40 degree divergent lines") {
    Setup setup = {0, 40, 10, 4, 6};
    System system(setup);
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(10, system)};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception, system, 10,
                      positions, speed_and_scale);
    CHECK(result.y == doctest::Approx(4.080766));
    CHECK(result.theta == doctest::Approx(-17.38014));
  }
  SUBCASE("Divergent line, less steep path than top line") {
    Setup setup = {0, 75, 18, 4, 8};
    System system(setup);
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(18, system)};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception, system, 18,
                      positions, speed_and_scale);
    CHECK(result.y == doctest::Approx(-2.467113));
    CHECK(result.theta == doctest::Approx(24.884769));
  }
  SUBCASE("Divergent lines almost extreme case") {
    Setup setup = {2, 80, 15, 3, 8};
    System system(setup);
    Point last_interception{0, 2};
    Point new_interception{calculateFirstHit(15, system)};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception, system, 15,
                      positions, speed_and_scale);
    CHECK(result.y == doctest::Approx(-5.934306));
    CHECK(result.theta == doctest::Approx(6.260205));
  }
  SUBCASE("Parallel lines") {
    Setup setup = {0, 45, 40, 5, 5};
    System system(setup);
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(40, system)};
    std::vector<Point> positions;
    Angle_and_point result =
        getFinalPoint(new_interception, last_interception, system, 40,
                      positions, speed_and_scale);
    CHECK(result.y == doctest::Approx(0));
    CHECK(result.theta == doctest::Approx(45));
  }
}
