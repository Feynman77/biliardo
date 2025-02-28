#ifndef BALL_H
#define BALL_H
#include <cmath>

class Ball {
 private:
  double m_theta_0{};
  double m_y_0{};

 public:
  double getTheta_0() const { return m_theta_0; }
  void setTheta_0(double);

  double getY_0() const { return m_theta_0; }
  void setY_0(double);

  Ball(double theta_0, double y_0) : m_theta_0{theta_0}, m_y_0{y_0} {}
  // double const slope(double m) { m = tan(m_theta_0); }
};

#endif