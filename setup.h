#ifndef SETUP_H
#define SETUP_H

#include <stdexcept>

#include "ForwardDeclaration.h"
#include "graphic.h"

class Setup {
private:
  float m_y_0;
  float m_theta_0;
  float m_l;
  float m_r_1;
  float m_r_2;

public:
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
};

#endif // SETUP_H