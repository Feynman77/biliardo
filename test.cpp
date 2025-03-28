#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "ForwardDeclaration.h"
#include "doctest.h"
TEST_CASE("DIO CANE") {
  SUBCASE("Straight throw") {
    Setup setup = {0, 0, 10, 4, 4};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(10, makeSystemFromSetup(setup))};
    std::vector<Point> positions;
    CHECK(getFinalPoint(
        new_interception, last_interception, makeSystemFromSetup(setup), 10, positions, 25, 25).y==doctest::Approx(0)); 
  }
}
