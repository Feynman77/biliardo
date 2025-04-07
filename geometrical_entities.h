#ifndef GEOMETRICAL_ENTITIES_H
#define GEOMETRICAL_ENTITIES_H
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <cmath>

struct Point {
  double x;
  double y;
};
struct Angle_and_point {
  double theta;
  double y;
};
struct Line {
  double slope;
  double q;

  Line(float slope_input, float q_input) : slope(slope_input), q(q_input) {}
  Line(const Point &p_1, const Point &p_2)
      : slope((p_2.y - p_1.y) / (p_2.x - p_1.x)), q(p_1.y - slope * p_1.x) {}
  Line(const Angle_and_point &a_p)
      : slope(tan(a_p.theta * M_PI / 180)), q(a_p.y) {}
};

struct Speed_and_scale {
  float speed;
  float scale;
};

class Setup {
 private:
  float m_y_0;
  float m_theta_0;
  float m_l;
  float m_r_1;
  float m_r_2;

 public:
  // constructor overload

  Setup(const tgui::Gui &gui);

  Setup(const float &y_0, const float &theta_0, const float &l,
        const float &r_1, const float &r_2);

  float get_y_0() const { return m_y_0; }
  float get_theta_0() const { return m_theta_0; }
  float get_l() const { return m_l; }
  float get_r_1() const { return m_r_1; }
  float get_r_2() const { return m_r_2; }

  void run(sf::CircleShape &ball, std::vector<Point> &positions,
           Speed_and_scale &speed_and_scale, tgui::Gui &gui,
           sf::VertexArray &top_line, sf::VertexArray &bottom_line);

  void getNormalDistribution(tgui::Gui &gui);

  void makeDrawableSystem(sf::CircleShape &ball, sf::VertexArray &top_line,
                          sf::VertexArray &bottom_line, const float &scale);
};

struct System {
  Line top_line;
  Line bottom_line;
  Line first_throw;

  // constructor overload
  System(Line top_line_input, Line bottom_line_input, Line first_throw_input)
      : top_line(top_line_input),
        bottom_line(bottom_line_input),
        first_throw(first_throw_input) {}

  System(const Setup &s)
      : top_line({0, s.get_r_1()}, {s.get_l(), s.get_r_2()}),
        bottom_line({0, -s.get_r_1()}, {s.get_l(), -s.get_r_2()}),
        first_throw(Angle_and_point{s.get_theta_0(), s.get_y_0()}) {}
};

#endif