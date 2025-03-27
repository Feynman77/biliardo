
#include <string>

#include "ForwardDeclaration.h"
#include <TH1F.h>
#include "TRandom.h"

// create the setup from user
Setup getParametersFromUser(const tgui::Gui &gui) {
  Setup s;
  s.r_1 = gui.get<tgui::EditBoxSlider>("r_1")->getValue();
  s.r_2 = gui.get<tgui::EditBoxSlider>("r_2")->getValue();
  s.l = gui.get<tgui::EditBoxSlider>("l")->getValue();
  float y_0 = gui.get<tgui::EditBoxSlider>("y_0")->getValue();
  float theta_0 = gui.get<tgui::EditBoxSlider>("theta_0")->getValue();
  s.y_0 = -y_0;
  s.theta_0 = -theta_0;
  return s;
}

// calculate m and q of a line from two point
// forse metodo di classe (costruttore in overload?)
Line makeLineFromPoints(const Point &p_1, const Point &p_2) {
  double slope{(p_2.y - p_1.y) / (p_2.x - p_1.x)};
  double q{p_1.y - slope * p_1.x};
  return Line(slope, q);
}

// calculate of m and q of a line from an angle
// idem sopra
Line makeLineFromAngle(const double &theta, const double &q) {
  double slope{tan(theta * M_PI / 180)};
  return Line(slope, q);
}

// find the interception beetween two lines
Point findInterception(const Line &l_1, const Line &l_2) {
  if (l_1.getSlope() != l_2.getSlope()) {
    double abscissa{(l_2.getQ() - l_1.getQ()) /
                    (l_1.getSlope() - l_2.getSlope())};
    double ordinate{l_1.getSlope() * abscissa + l_1.getQ()};
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
    double result{system.first_throw.getSlope() * l +
                  system.first_throw.getQ()};
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
      double x =
          last_interception.x + i * (speed / 30) * cos(atan(path.getSlope()));
      Point position{scale * x,
                     scale * path.getSlope() * x + scale * path.getQ()};

      positions.emplace_back(position);
      i++;
    } while ((positions.back().x <= scale * new_interception.x) &&
             (positions.back().x <= scale * l));
  } else {
    do {
      double x =
          last_interception.x - i * (speed / 30) * cos(atan(path.getSlope()));
      Point position{scale * x,
                     scale * path.getSlope() * x + scale * path.getQ()};

      positions.emplace_back(position);
      i++;
    } while ((positions.back().x >= scale * new_interception.x) &&
             (positions.back().x >= 0));
  }
}

// calculate the final point
void getFinalPoint(Point &new_interception, Point &last_interception,
                   const System &system, const double &l,
                   std::vector<Point> &positions, const double &speed,
                   const double &scale, tgui::Gui &gui) {
  Line path(system.first_throw.getSlope(), system.first_throw.getQ());

  // ball bounces
  while (last_interception.x <= new_interception.x) {
    // valid throw final path
    if (new_interception.x >= l) {
      fillVector(positions, last_interception, new_interception, path, speed,
                 scale, l);
      double result{path.getSlope() * l +
                    path.getQ()};  // wrong sign for no reason
      double final_angle{atan(path.getSlope())};
      gui.get<tgui::EditBox>("Final angle")
          ->setText(std::to_string(-final_angle));
      gui.get<tgui::EditBox>("Final point")
          ->setText((std::to_string(l) + "; " + std::to_string(-result)));
      break;
    };

    // ball hits top line first (Francesco)
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y > 0) {
      // we find the path after hitting the top line
      fillVector(positions, last_interception, new_interception, path, speed,
                 scale, l);
      path.setSlope((path.getSlope() * system.top_line.getSlope() *
                         system.top_line.getSlope() -
                     path.getSlope() + 2 * system.top_line.getSlope()) /
                    (1 -
                     system.top_line.getSlope() * system.top_line.getSlope() +
                     2 * path.getSlope() * system.top_line.getSlope()));
      path.setQ(new_interception.y - path.getSlope() * new_interception.x);

      last_interception = new_interception;
      new_interception = findInterception(path, system.bottom_line);
    };

    // ball hits bottom line first (Francesco)
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y < 0) {
      fillVector(positions, last_interception, new_interception, path, speed,
                 scale, l);
      // we find the path after hitting the bottom line
      path.setSlope(-path.getSlope());
      path.setSlope((-1) *
                    (path.getSlope() * system.top_line.getSlope() *
                         system.top_line.getSlope() -
                     path.getSlope() + 2 * system.top_line.getSlope()) /
                    (1 -
                     system.top_line.getSlope() * system.top_line.getSlope() +
                     2 * path.getSlope() * system.top_line.getSlope()));
      path.setQ(new_interception.y - path.getSlope() * new_interception.x);

      last_interception = new_interception;
      new_interception = findInterception(path, system.top_line);
    };

    if ((system.top_line.getSlope() > system.bottom_line.getSlope()) &&
        (new_interception.x < 0)) {
      new_interception.y = path.getSlope() * l + path.getQ();
      new_interception.x = l;
      fillVector(positions, last_interception, new_interception, path, speed,
                 scale, l);
      double result{path.getSlope() * l +
                    path.getQ()};  // wrong sign for no reason
      double final_angle{atan(path.getSlope())};
      gui.get<tgui::EditBox>("Final angle")
          ->setText(std::to_string(-final_angle));
      gui.get<tgui::EditBox>("Final point")
          ->setText((std::to_string(l) + "; " + std::to_string(result)));
      break;
    };

    // ball goes back
    if (new_interception.x < last_interception.x) {
      gui.get<tgui::EditBox>("Final angle")->setText("Invalid throw");
      gui.get<tgui::EditBox>("Final point")->setText("Invalid throw");
      while (last_interception.x >= new_interception.x &&
             last_interception.x > 0) {
        // ball hits top line first (Francesco)
        if (new_interception.x < l && new_interception.y > 0 &&
            last_interception.x >= 0) {
          // we find the path after hitting the top line
          fillVector(positions, last_interception, new_interception, path,
                     speed, scale, l);
          path.setSlope(
              (path.getSlope() * system.top_line.getSlope() *
                   system.top_line.getSlope() -
               path.getSlope() + 2 * system.top_line.getSlope()) /
              (1 - system.top_line.getSlope() * system.top_line.getSlope() +
               2 * path.getSlope() * system.top_line.getSlope()));
          path.setQ(new_interception.y - path.getSlope() * new_interception.x);

          last_interception = new_interception;
          new_interception = findInterception(path, system.bottom_line);
        };

        // ball hits bottom line first (Francesco)
        if (new_interception.x < l && new_interception.y < 0 &&
            last_interception.x >= 0) {
          fillVector(positions, last_interception, new_interception, path,
                     speed, scale, l);
          // we find the path after hitting the bottom line
          path.setSlope(-path.getSlope());
          path.setSlope(
              (-1) *
              (path.getSlope() * system.top_line.getSlope() *
                   system.top_line.getSlope() -
               path.getSlope() + 2 * system.top_line.getSlope()) /
              (1 - system.top_line.getSlope() * system.top_line.getSlope() +
               2 * path.getSlope() * system.top_line.getSlope()));
          path.setQ(new_interception.y - path.getSlope() * new_interception.x);

          last_interception = new_interception;
          new_interception = findInterception(path, system.top_line);
        };
      };
    };
  }
}

// we arrived here

void calculateFinalPoint(Point &new_interception, Point &last_interception,
                         const System &system, const double &l, TH1F &h1,
                         TH1F &h2) {
  Line path(system.first_throw.getSlope(), system.first_throw.getQ());

  // ball bounces (maybe)

  while (last_interception.x <= new_interception.x) {
    // valid throw final path
    if (new_interception.x >= l) {
      double result{-(path.getSlope() * l + path.getQ())};
      double final_angle{-atan(path.getSlope())};
      h1.Fill(result);
      h2.Fill(final_angle);
      break;
    };

    // ball hits top line first (Francesco)
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y > 0) {
      // we find the path after hitting the top line

      path.setSlope((path.getSlope() * system.top_line.getSlope() *
                         system.top_line.getSlope() -
                     path.getSlope() + 2 * system.top_line.getSlope()) /
                    (1 -
                     system.top_line.getSlope() * system.top_line.getSlope() +
                     2 * path.getSlope() * system.top_line.getSlope()));
      path.setQ(new_interception.y - path.getSlope() * new_interception.x);
      last_interception = new_interception;

      new_interception = findInterception(path, system.bottom_line);
    };

    // ball hits bottom line first (Francesco)
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y < 0) {
      // we find the path after hitting the bottom line
      path.setSlope(-path.getSlope());
      path.setSlope((-1) *
                    (path.getSlope() * system.top_line.getSlope() *
                         system.top_line.getSlope() -
                     path.getSlope() + 2 * system.top_line.getSlope()) /
                    (1 -
                     system.top_line.getSlope() * system.top_line.getSlope() +
                     2 * path.getSlope() * system.top_line.getSlope()));
      path.setQ(new_interception.y - path.getSlope() * new_interception.x);

      last_interception = new_interception;
      new_interception = findInterception(path, system.top_line);
    };
    if ((system.top_line.getSlope() > system.bottom_line.getSlope()) &&
        (new_interception.x < 0)) {
      new_interception.y = path.getSlope() * l + path.getQ();
      new_interception.x = l;
      double result{path.getSlope() * l +
                    path.getQ()};  // wrong sign for no reason
      double final_angle{atan(path.getSlope())};
      h1.Fill(result);
      h2.Fill(final_angle);
      break;
    };
    // ball goes back
    if (new_interception.x < last_interception.x) {
      break;
    };
  }
}

void getNormalDistribution(const Setup &setup, tgui::Gui &gui) {
  float sigma_y_0 = gui.get<tgui::EditBoxSlider>("sigma y_0")->getValue();
  float sigma_theta_0 =
      gui.get<tgui::EditBoxSlider>("sigma theta_0")->getValue();
  int n = static_cast<int>(gui.get<tgui::EditBoxSlider>("n")->getValue());

  TH1F h1("Isto1", "Final points", 20, -setup.r_2, setup.r_2);
  TH1F h2("Isto2", "Final angles", 100, -M_PI, M_PI);

  for (int i{0}; i < n; i++) {
    float theta =
        static_cast<float>(gRandom->Gaus(setup.theta_0, sigma_theta_0));
    float y = static_cast<float>(gRandom->Gaus(setup.y_0, sigma_y_0));
    Setup setup_gaus{y, theta, setup.l, setup.r_1, setup.r_2};
    System system(makeSystemFromSetup(setup_gaus));

    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(setup_gaus.l, system)};

    calculateFinalPoint(new_interception, last_interception, system,
                        setup_gaus.l, h1, h2);
  }

  TCanvas canvas{};
  canvas.Divide(2, 1);
  canvas.cd(1);
  h1.Draw();
  canvas.cd(2);
  h2.Draw();
  canvas.Print("histos.png");

  gui.get<tgui::EditBox>("Position mean")
      ->setText(std::to_string(h1.GetMean()));
  gui.get<tgui::EditBox>("Position stddev")
      ->setText(std::to_string(h1.GetRMS()));
  gui.get<tgui::EditBox>("Position skewedness")
      ->setText(std::to_string(h1.GetSkewness()));
  gui.get<tgui::EditBox>("Position kurtosis")
      ->setText(std::to_string(h1.GetKurtosis()));

  gui.get<tgui::EditBox>("Angle mean")->setText(std::to_string(h2.GetMean()));
  gui.get<tgui::EditBox>("Angle stddev")->setText(std::to_string(h2.GetRMS()));
  gui.get<tgui::EditBox>("Angle skewedness")
      ->setText(std::to_string(h2.GetSkewness()));
  gui.get<tgui::EditBox>("Angle kurtosis")
      ->setText(std::to_string(h2.GetKurtosis()));
}