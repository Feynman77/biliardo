
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

// find the interception beetween two lines
Point findInterception(const Line &l_1, const Line &l_2)
{
  if (l_1.slope != l_2.slope)
  {
    double abscissa{(l_2.q - l_1.q) / (l_1.slope - l_2.slope)};
    double ordinate{l_1.slope * abscissa + l_1.q};
    Point p;
    p.x = abscissa;
    p.y = ordinate;
    return p;
  }
  else
  {
    Point p{-1, 0}; // x that gets discarded by main
    return p;
  }
}

// making system from setup
// System makeSystemFromSetup(const Setup &s)
// {
//   Line top_line(makeLineFromPoints({0, s.get_r_1()}, {s.get_l(), s.get_r_2()}));
//   Line bottom_line(makeLineFromPoints({0, -s.get_r_1()}, {s.get_l(), -s.get_r_2()}));
//   Line first_throw(makeLineFromAngle(s.get_theta_0(), s.get_y_0()));
//   System system{top_line, bottom_line, first_throw};
//   return system;
// }

// calculate of the first hit
Point calculateFirstHit(const double &l, const System &system)
{
  // defining logical points
  Point new_interception;
  Point interception_top_line{
      findInterception(system.top_line, system.first_throw)};
  Point interception_bottom_line{
      findInterception(system.bottom_line, system.first_throw)};

  // ball hits top line
  if (interception_top_line.x >= 0 && interception_top_line.x <= l)
  {
    new_interception = interception_top_line;
  };

  // ball hits bottom line
  if (interception_bottom_line.x >= 0 && interception_bottom_line.x <= l)
  {
    new_interception = interception_bottom_line;
  };

  // ball goes directly out
  if ((interception_top_line.x > l && interception_bottom_line.x < 0) ||
      (interception_bottom_line.x > l && interception_top_line.x < 0) ||
      (interception_top_line.x > l && interception_bottom_line.x > l) ||
      (interception_top_line.x < 0 && interception_bottom_line.x < 0))
  {
    double result{system.first_throw.slope * l + system.first_throw.q};
    new_interception.x = l;
    new_interception.y = result;
  };

  return new_interception;
}

// filling the vector with the positions of the motion
void fillVector(std::vector<Point> &positions, const Point &last_interception,
                const Point &new_interception, const Line &path,
                const Speed_and_scale &speed_and_scale, const double &l)
{
  int i{0};
  if (last_interception.x < new_interception.x)
  {
    do
    {
      double x{last_interception.x +
               static_cast<float>(i) * (speed_and_scale.speed / 30.f) * cos(atan(path.slope))};
      Point position{speed_and_scale.scale * x,
                     speed_and_scale.scale * path.slope * x +
                         speed_and_scale.scale * path.q};

      positions.emplace_back(position);
      i++;
    } while (
        (positions.back().x <= speed_and_scale.scale * new_interception.x) &&
        (positions.back().x <= speed_and_scale.scale * l));
  }
  else
  {
    do
    {
      double x{last_interception.x -
               static_cast<float>(i) * (speed_and_scale.speed / 30.f) * cos(atan(path.slope))};
      Point position{speed_and_scale.scale * x,
                     speed_and_scale.scale * path.slope * x +
                         speed_and_scale.scale * path.q};

      positions.emplace_back(position);
      i++;
    } while (
        (positions.back().x >= speed_and_scale.scale * new_interception.x) &&
        (positions.back().x >= 0));
  }
}

// calculate the final point
Angle_and_point getFinalPoint(Point &new_interception, Point &last_interception,
                              const System &system, const double &l,
                              std::vector<Point> &positions,
                              const Speed_and_scale &speed_and_scale)
{
  // setting the initial situation
  Line path(system.first_throw.slope, system.first_throw.q);
  Angle_and_point p;

  // ball start bouncing
  while (last_interception.x <= new_interception.x)
  {
    // valid final throw (give us the result)
    if (new_interception.x >= l)
    {
      fillVector(positions, last_interception, new_interception, path,
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
        new_interception.y > 0)
    {
      fillVector(positions, last_interception, new_interception, path,
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
        new_interception.y < 0)
    {
      fillVector(positions, last_interception, new_interception, path,
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
        (new_interception.x < 0))
    {
      new_interception.y = path.slope * l + path.q;
      new_interception.x = l;
      fillVector(positions, last_interception, new_interception, path,
                 speed_and_scale, l);

      double result{path.slope * l + path.q}; // wrong sign for a reason
      double final_angle{atan(path.slope)};

      p.theta = -final_angle * 180 / M_PI;
      p.y = -result;
      break;
    };
  }
  // ball goes back
  if (new_interception.x < last_interception.x)
  {
    while (last_interception.x >= new_interception.x &&
           last_interception.x > 0)
    {
      // ball hits top line
      if (new_interception.x < l && new_interception.y > 0 &&
          last_interception.x >= 0)
      {
        // finding path after the hit of the ball with the top line
        fillVector(positions, last_interception, new_interception, path,
                   speed_and_scale, l);
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
          last_interception.x >= 0)
      {
        fillVector(positions, last_interception, new_interception, path,
                   speed_and_scale, l);
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
                         TH1F &h2)
{
  Line path(system.first_throw.slope, system.first_throw.q);

  // ball starts bouncing
  while (last_interception.x <= new_interception.x)
  {
    // valid throw final path
    if (new_interception.x >= l)
    {
      double result{-(path.slope * l + path.q)};
      double final_angle{-atan(path.slope)};
      h1.Fill(result);
      h2.Fill(final_angle);
      break;
    };

    // ball hits top line
    if (new_interception.x < l && new_interception.x >= 0 &&
        new_interception.y > 0)
    {
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
        new_interception.y < 0)
    {
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
        (new_interception.x < 0))
    {
      new_interception.y = path.slope * l + path.q;
      new_interception.x = l;
      double result{path.slope * l + path.q}; // wrong sign for a reason
      double final_angle{atan(path.slope)};
      h1.Fill(result);
      h2.Fill(final_angle);
      break;
    };

    // ball goes back
    if (new_interception.x < last_interception.x)
    {
      break;
    };
  }
}

// drowing the histograms of the normal distributions
void getNormalDistribution(const Setup &setup, tgui::Gui &gui)
{
  // defining the inputs from the sliders
  float sigma_y_0 = gui.get<tgui::EditBoxSlider>("sigma y_0")->getValue();
  float sigma_theta_0 =
      gui.get<tgui::EditBoxSlider>("sigma theta_0")->getValue();
  int n = static_cast<int>(gui.get<tgui::EditBoxSlider>("n")->getValue());

  // creation of the histograms
  TH1F h1("Isto1", "Final points", 20, -setup.get_r_2(), setup.get_r_2());
  TH1F h2("Isto2", "Final angles", 100, -M_PI, M_PI);

  // filling the histograms n times
  for (int i{0}; i < n; i++)
  {
    float theta =
        static_cast<float>(gRandom->Gaus(setup.get_theta_0(), sigma_theta_0));
    float y = static_cast<float>(gRandom->Gaus(setup.get_y_0(), sigma_y_0));
    Setup setup_gaus{y, theta, setup.get_l(), setup.get_r_1(), setup.get_r_2()};
    System system(setup_gaus);

    Point last_interception{0, 0};
    Point new_interception{calculateFirstHit(setup_gaus.get_l(), system)};

    calculateFinalPoint(new_interception, last_interception, system,
                        setup_gaus.get_l(), h1, h2);
  }

  // drowing histograms and final outputs on the Gui
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

// running the program
void run(sf::CircleShape &ball, std::vector<Point> &positions,
         Speed_and_scale &speed_and_scale, tgui::Gui &gui,
         sf::VertexArray &top_line, sf::VertexArray &bottom_line)
{
  positions.clear();
  Setup setup = (gui);

  // rescaling speed and scale  in base  of the  higest parameter
  float scale_reference =
      std::max({setup.get_l() / 30, setup.get_r_1() / 8, setup.get_r_2() / 8});
  speed_and_scale.scale = 25 / scale_reference;
  speed_and_scale.speed = 1.5f * scale_reference;
  setup.makeDrawableSystem(ball, top_line, bottom_line, speed_and_scale.scale);

  // FIXME Perché non è un costruttore??
  System system(setup);

  Point last_interception{0, 0};
  Point new_interception{calculateFirstHit(setup.get_l(), system)};

  Angle_and_point result =
      getFinalPoint(new_interception, last_interception, system, setup.get_l(),
                    positions, speed_and_scale);
  if (result.y == 0 && result.theta == 180)
  {
    gui.get<tgui::EditBox>("Final angle")->setText("Invalid throw");
    gui.get<tgui::EditBox>("Final point")->setText("Invalid throw");
  }
  else
  {
    gui.get<tgui::EditBox>("Final angle")
        ->setText(std::to_string(result.theta));
    gui.get<tgui::EditBox>("Final point")
        ->setText(std::to_string(setup.get_l()) + ";" + std::to_string(result.y));
  }
}

// setting the valid physical range for y_0 values
void sliderUpdate(tgui::Gui &gui)
{
  float r_1 = gui.get<tgui::EditBoxSlider>("r_1")->getValue();
  gui.get<tgui::EditBoxSlider>("y_0")->setMaximum(r_1 - 0.01f);
  gui.get<tgui::EditBoxSlider>("y_0")->setMinimum(-r_1 + 0.01f);
}