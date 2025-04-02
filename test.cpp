#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "graphic.h"
#include "ForwardDeclaration.h"
#include "cmath"
#include "doctest.h"

TEST_CASE("Test findInterception") {
  SUBCASE("upwards line with downwards line") {
    Line l_1{makeLineFromPoints({0, 4}, {7, 5})};
    Line l_2{makeLineFromAngle(45, 3)};
    Point result = findInterception(l_1, l_2);
    CHECK(result.x == doctest::Approx(1.167).epsilon(0.001));
    CHECK(result.y == doctest::Approx(4.167).epsilon(0.001));
  }
  SUBCASE("downwards line with upwards line") {
    Line l_1{makeLineFromPoints({0, -2}, {10, -1})};
    Line l_2{makeLineFromAngle(-45, -1)};
    Point result = findInterception(l_1, l_2);
    CHECK(result.x == doctest::Approx(0.91).epsilon(0.001));
    CHECK(result.y == doctest::Approx(-1.91).epsilon(0.001));
  }
}

TEST_CASE("Test calculateFirstHit") {
  SUBCASE("no line") {
    double l = 4;
    Setup s{-3, 23.2f, 4, 4, 2};
    System system = makeSystemFromSetup(s);
    Point result = calculateFirstHit(l, system);
    CHECK(result.x == doctest::Approx(4).epsilon(0.01));
    CHECK(result.y == doctest::Approx(-1.29).epsilon(0.01));
  }
  SUBCASE("hit top line") {
    double l = 6;
    Setup s{1, 56.31f, 6, 4, 3};
    System system = makeSystemFromSetup(s);
    Point result = calculateFirstHit(l, system);
    CHECK(result.x == doctest::Approx(1.8).epsilon(0.01));
    CHECK(result.y == doctest::Approx(3.7).epsilon(0.01));
  }
  SUBCASE("hit bottom line") {
    double l = 10;
    Setup s{-1, -20.03f, 10, 4, 2.5};
    System system = makeSystemFromSetup(s);
    Point result = calculateFirstHit(l, system);
    CHECK(result.x == doctest::Approx(5.83).epsilon(0.01));
    CHECK(result.y == doctest::Approx(-3.13).epsilon(0.01));
  }
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
    CHECK(result.y == doctest::Approx(2.467113));
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
