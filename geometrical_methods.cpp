
#include "geometrical_methods.h"

#include <TH1F.h>

#include <string>

#include "TRandom.h"

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
void fillVector(std::vector<Point> &animation_positions,
                const Point &last_interception, const Point &new_interception,
                const Line &path, const Speed_and_scale &speed_and_scale,
                const double &l) {
  int i{0};
  if (last_interception.x < new_interception.x) {
    // it has to do at least one iteration to work
    do {
      // generate a position for every frame
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
    // same idea, different formula because it moves backwards
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
      // flipped system because sfml's ordinate is flipped)
      double result{path.slope * l + path.q};
      double final_angle{atan(path.slope)};

      p.theta = -final_angle * 180 / M_PI;
      p.y = -result;
      break;
    };

    // ball hits (sfml's) top line
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

    // ball hits (sfml's) bottom line
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

      double result{path.slope * l +
                    path.q};  // wrong sign for same reason as before
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
      // ball hits (sfml's) top line
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

      // ball hits (sfml's)bottom line
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
void nThrowsSimulation(Point &new_interception, Point &last_interception,
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
      // filling the histograms
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
      // filling the histograms
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