#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#include "ForwardDeclaration.h"
/* #include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TRandom.h" */
#include "graphic.h"
#include "line.h"

Setup getParametersFromUser() {
  Setup s;
  std::cout << "Insert y_0 and theta_0" << '\n';
  std::cin >> s.y_0 >> s.theta_0;
  std::cout << "Insert l, r_1 and r_2" << '\n';
  std::cin >> s.l >> s.r_1 >> s.r_2;
  return s;
}

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

System makeSystemFromSetup(Setup s) {
  Line top_line(makeLineFromPoints({0, s.r_1}, {s.l, s.r_2}));
  Line bottom_line(makeLineFromPoints({0, -s.r_1}, {s.l, -s.r_2}));
  Line first_throw(makeLineFromAngle(s.theta_0, s.y_0));
  System system{top_line, bottom_line, first_throw};
  return system;
}

Point calculateFirstHit(Point interception_top_line,
                        Point interception_bottom_line, Setup setup,
                        System system) {
  Point new_interception;

  // ball hits top line
  if (interception_top_line.x <= setup.l && interception_top_line.x >= 0) {
    new_interception = interception_top_line;
    return new_interception;
  };

  // ball hits bottom line
  if (interception_bottom_line.x <= setup.l &&
      interception_bottom_line.x >= 0) {
    new_interception = interception_bottom_line;
    return new_interception;
  };

  // ball goes directly out
  if ((interception_top_line.x > setup.l && interception_bottom_line.x < 0) ||
      (interception_bottom_line.x > setup.l && interception_top_line.x < 0) ||
      (interception_top_line.x > setup.l &&
       interception_bottom_line.x > setup.l) ||
      (interception_top_line.x < 0 && interception_bottom_line.x < 0)) {
    double result{system.first_throw.getSlope() * setup.l +
                  system.first_throw.getQ()};
    new_interception.x = setup.l;
    new_interception.y = result;
    return new_interception;
  };
  return new_interception;
}

std::vector<Point> fillVector(std::vector<Point> positions,
                              Point last_interception, Point new_interception,
                              Line path, double speed) {
  do {
    double i{0};
    double x =
        last_interception.x + i * (speed / 30) * cos(atan(path.getSlope()));
    Point position{x, path.getSlope() * x + path.getQ()};

    positions.emplace_back(position);
  } while (positions.back().x < new_interception.x);

  return positions;
}

Point getFinalPoint(Point new_interception, Point last_interception,
                    System system, Setup setup, std::vector<Point> positions) {
  Line path(system.first_throw.getSlope(), system.first_throw.getQ());

  // ball bounces (maybe)

  while (last_interception.x <= new_interception.x) {
    std::cout << "While loop" << '\n';

    // valid throw final path
    if (new_interception.x >= setup.l) {
      fillVector(positions, last_interception, new_interception, path, 25);
      double result{path.getSlope() * setup.l + path.getQ()};
      std::cout << " The final coordinates are x=" << setup.l
                << " and y=" << result << '\n';
      break;
    };

    // ball hits top line first (Francesco)
    if (new_interception.x < setup.l && new_interception.x >= 0 &&
        new_interception.y > 0) {
      // we find the path after hitting the top line
      fillVector(positions, last_interception, new_interception, path, 25);
      path.setSlope(
          (path.getSlope() * system.top_line.getSlope() *
               system.top_line.getSlope() -
           path.getSlope() - 2 * std::abs(system.top_line.getSlope())) /
          (1 - system.top_line.getSlope() * system.top_line.getSlope() -
           2 * path.getSlope() * std::abs(system.top_line.getSlope())));
      path.setQ(new_interception.y - path.getSlope() * new_interception.x);
      last_interception = new_interception;

      new_interception = findInterception(path, system.bottom_line);

      std::cout << "Top line: hit" << '\n';
    };

    // ball hits bottom line first (Francesco)
    if (new_interception.x < setup.l && new_interception.x >= 0 &&
        new_interception.y < 0) {
      fillVector(positions, last_interception, new_interception, path, 25);
      // we find the path after hitting the bottom line
      path.setSlope(-path.getSlope());
      path.setSlope(
          (-1) *
          (path.getSlope() * system.top_line.getSlope() *
               system.top_line.getSlope() -
           path.getSlope() - 2 * std::abs(system.top_line.getSlope())) /
          (1 - system.top_line.getSlope() * system.top_line.getSlope() -
           2 * path.getSlope() * std::abs(system.top_line.getSlope())));
      path.setQ(new_interception.y - path.getSlope() * new_interception.x);

      last_interception = new_interception;
      new_interception = findInterception(path, system.top_line);
      std::cout << "Bottom line: hit" << '\n';
    };

    // ball goes back
    if (new_interception.x < last_interception.x) {
      std::cout
          << "Invalid throw. The ball went behind the starting line. Try again."
          << '\n';
      break;
    };
  }

  Point p{};
  return p;
}

/*void getNormalDistribution(Setup setup) {
  double sigma_y;
  double sigma_theta;
  int n;
  TH1F *h1 = new TH1F("Isto1", "Final points", 200, -10, 10);

  std::cout << "Insert sigma y_0, sigma theta_0 and number of iterations."
            << '\n';
  std::cin >> sigma_y >> sigma_theta >> n;

  for (int i{0}; i < n; i++) {
    double theta = gRandom->Gaus(setup.theta_0, sigma_theta);
    double y = gRandom->Gaus(setup.y_0, sigma_y);
    Setup setup_gaus{y, theta, setup.l, setup.r_1, setup.r_2};
    System system(makeSystemFromSetup(setup_gaus));

    Point interception_top_line{
        findInterception(system.top_line, system.first_throw)};
    Point interception_bottom_line{
        findInterception(system.bottom_line, system.first_throw)};
    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(
        interception_top_line, interception_bottom_line, setup_gaus, system)};

    Point final_point{
        getFinalPoint(new_interception, last_interception, system, setup_gaus)};

    h1->Fill(final_point.y);
  }

  // h1->Fit("gaus");
  double_t mean{h1->GetMean()};
  std::cout << "Mean = " << mean << '\n';
  std::cout << "STDeav = " << h1->GetRMS() << '\n';
  std::cout << "Skewness: " << h1->GetSkewness() << '\n';
  std::cout << "Kurtosis: " << h1->GetKurtosis() << '\n';
  std::cout << "Entries in the histogram: " << h1->GetEntries() << std::endl;

  h1->Draw();
}*/