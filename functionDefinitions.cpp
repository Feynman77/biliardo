
#include <TH1F.h>

#include <string>

#include "ForwardDeclaration.h"
#include "TRandom.h"

// creation of the setup from user
// Setup getParametersFromUser(const tgui::Gui &gui) {
//   Setup s;
//   s.r_1 = gui.get<tgui::EditBoxSlider>("r_1")->getValue();
//   s.r_2 = gui.get<tgui::EditBoxSlider>("r_2")->getValue();
//   s.l = gui.get<tgui::EditBoxSlider>("l")->getValue();
//   float y_0 = gui.get<tgui::EditBoxSlider>("y_0")->getValue();
//   float theta_0 = gui.get<tgui::EditBoxSlider>("theta_0")->getValue();
//   s.y_0 = -y_0;
//   s.theta_0 = -theta_0;
//   return s;
// }

// calculate slope and q of a line from two point
// Line makeLineFromPoints(const Point &p_1, const Point &p_2)
// {
//   double slope{(p_2.y - p_1.y) / (p_2.x - p_1.x)};
//   double q{p_1.y - slope * p_1.x};
//   return Line{slope, q};
// }

// // calculate of slope and q of a line from an angle
// Line makeLineFromAngle(const double &theta, const double &q)
// {
//   double slope{tan(theta * M_PI / 180)};
//   return Line{slope, q};
// }

// finding the interception beetween two lines
Point findInterception(const Line &l_1, const Line &l_2) {
  if (l_1.slope != l_2.slope) {
    double abscissa{(l_2.q - l_1.q) / (l_1.slope - l_2.slope)};
    double ordinate{l_1.slope * abscissa + l_1.q};
    Point p;
    p.x = abscissa;
    p.y = ordinate;
    return p;
  } else {
    // parallel lines, giving x that gets discarded by the main
    Point p{-1, 0};
    return p;
  }
}

// making system from setup
// System makeSystemFromSetup(const Setup &s)
// {
//   Line top_line(makeLineFromPoints({0, s.get_r_1()}, {s.get_l(),
//   s.get_r_2()})); Line bottom_line(makeLineFromPoints({0, -s.get_r_1()},
//   {s.get_l(), -s.get_r_2()})); Line
//   first_throw(makeLineFromAngle(s.get_theta_0(), s.get_y_0())); System
//   system{top_line, bottom_line, first_throw}; return system;
// }

// calculate of the first hit
Point calculateFirstHit(const double &l, const System &system) {
  // defining logical points
  Point new_interception;
  Point interception_top_line{
      findInterception(system.top_line, system.first_throw)};
  Point interception_bottom_line{
      findInterception(system.bottom_line, system.first_throw)};

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

// filling the vector with the positions of the motion
// FORSE COMMENTARE ALTRE COSE
void fillVector(std::vector<Point> &animation_positions,
                const Point &last_interception, const Point &new_interception,
                const Line &path, const Speed_and_scale &speed_and_scale,
                const double &l) {
  int i{0};
  if (last_interception.x < new_interception.x) {
    do {
      double x{last_interception.x + static_cast<float>(i) *
                                         (speed_and_scale.speed / 30.f) *
                                         cos(atan(path.slope))};
      Point position{speed_and_scale.scale * x,
                     speed_and_scale.scale * path.slope * x +
                         speed_and_scale.scale * path.q};

      animation_positions.emplace_back(position);
      i++;
    } while ((animation_positions.back().x <=
              speed_and_scale.scale * new_interception.x) &&
             (animation_positions.back().x <= speed_and_scale.scale * l));
  } else {
    do {
      double x{last_interception.x - static_cast<float>(i) *
                                         (speed_and_scale.speed / 30.f) *
                                         cos(atan(path.slope))};
      Point position{speed_and_scale.scale * x,
                     speed_and_scale.scale * path.slope * x +
                         speed_and_scale.scale * path.q};

      animation_positions.emplace_back(position);
      i++;
    } while ((animation_positions.back().x >=
              speed_and_scale.scale * new_interception.x) &&
             (animation_positions.back().x >= 0));
  }
}

// calculate the final point
Angle_and_point getFinalPoint(Point &new_interception, Point &last_interception,
                              const System &system, const double &l,
                              std::vector<Point> &animation_positions,
                              const Speed_and_scale &speed_and_scale) {
  // setting the initial situation
  Line path(static_cast<float>(system.first_throw.slope),
            static_cast<float>(system.first_throw.q));
  Angle_and_point p;

  // ball start bouncing
  while (last_interception.x <= new_interception.x) {
    // valid final throw (give us the result)
    if (new_interception.x >= l) {
      fillVector(animation_positions, last_interception, new_interception, path,
                 speed_and_scale, l);

      // calculate the final informations
      // wrong sign but there is a reason ( we do  the calculations on the
      // flipped system because sfml is flipped)
      double result{path.slope * l + path.q};
      double final_angle{atan(path.slope)};

      p.theta = -final_angle * 180 / M_PI;
      p.y = -result;
      break;
    };

    // ball hits top line
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y > 0) {
      fillVector(animation_positions, last_interception, new_interception, path,
                 speed_and_scale, l);

      // finding path after the hit of the ball with the top line
      path.slope =
          ((path.slope * system.top_line.slope * system.top_line.slope -
            path.slope + 2 * system.top_line.slope) /
           (1 - system.top_line.slope * system.top_line.slope +
            2 * path.slope * system.top_line.slope));
      path.q = (new_interception.y - path.slope * new_interception.x);

      last_interception = new_interception;
      new_interception = findInterception(path, system.bottom_line);
    };

    // ball hits bottom line
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y < 0) {
      fillVector(animation_positions, last_interception, new_interception, path,
                 speed_and_scale, l);

      // finding path after the hit of the ball with the bottom line
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

    // valide case with divergent lines
    if ((system.top_line.slope > system.bottom_line.slope) &&
        (new_interception.x < 0)) {
      new_interception.y = path.slope * l + path.q;
      new_interception.x = l;
      fillVector(animation_positions, last_interception, new_interception, path,
                 speed_and_scale, l);

      double result{path.slope * l + path.q};  // wrong sign for a reason
      double final_angle{atan(path.slope)};

      p.theta = -final_angle * 180 / M_PI;
      p.y = -result;
      break;
    };
  }

  // ball goes back
  if (new_interception.x < last_interception.x) {
    while (last_interception.x >= new_interception.x &&
           last_interception.x > 0) {
      // ball hits top line
      if (new_interception.x < l && new_interception.y > 0 &&
          last_interception.x >= 0) {
        // finding path after the hit of the ball with the top line
        fillVector(animation_positions, last_interception, new_interception,
                   path, speed_and_scale, l);
        path.slope =
            ((path.slope * system.top_line.slope * system.top_line.slope -
              path.slope + 2 * system.top_line.slope) /
             (1 - system.top_line.slope * system.top_line.slope +
              2 * path.slope * system.top_line.slope));
        path.q = (new_interception.y - path.slope * new_interception.x);

        last_interception = new_interception;
        new_interception = findInterception(path, system.bottom_line);
      };

      // ball hits bottom line
      if (new_interception.x < l && new_interception.y < 0 &&
          last_interception.x >= 0) {
        fillVector(animation_positions, last_interception, new_interception,
                   path, speed_and_scale, l);
        // finding the path after hitting the bottom line
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
  return p;
}

// calculation of the final point and filling the histograms
void calculateFinalPoint(Point &new_interception, Point &last_interception,
                         const System &system, const double &l, TH1F &h1,
                         TH1F &h2) {
  Line path(static_cast<float>(system.first_throw.slope),
            static_cast<float>(system.first_throw.q));

  // ball starts bouncing
  while (last_interception.x <= new_interception.x) {
    // valid throw final path
    if (new_interception.x >= l) {
      double result{(path.slope * l + path.q)};
      double final_angle{atan(path.slope) * 180 / M_PI};
      h1.Fill(result);
      h2.Fill(final_angle);
      break;
    };

    // ball hits top line
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y > 0) {
      // finding the path after hitting the top line
      path.slope =
          ((path.slope * system.top_line.slope * system.top_line.slope -
            path.slope + 2 * system.top_line.slope) /
           (1 - system.top_line.slope * system.top_line.slope +
            2 * path.slope * system.top_line.slope));
      path.q = (new_interception.y - path.slope * new_interception.x);

      last_interception = new_interception;
      new_interception = findInterception(path, system.bottom_line);
    };

    // ball hits bottom line
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y < 0) {
      // finding the path after hitting the bottom line
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
      new_interception.y = path.slope * l + path.q;
      new_interception.x = l;
      double result{path.slope * l + path.q};
      double final_angle{atan(path.slope) * 180 / M_PI};
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

// setting the valid physical range for y_0 values
void sliderUpdate(tgui::Gui &gui) {
  float r_1 = gui.get<tgui::EditBoxSlider>("r_1")->getValue();
  gui.get<tgui::EditBoxSlider>("y_0")->setMaximum(r_1 - 0.01f);
  gui.get<tgui::EditBoxSlider>("y_0")->setMinimum(-r_1 + 0.01f);
}