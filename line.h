#ifndef LINE_H
#define LINE_H

class Line {
 private:
  double m_slope{};
  double m_q{};

 public:
  double getSlope() const { return m_slope; }
  void setSlope(double);

  double getQ() const { return m_q; }
  void setQ(double);

  /*line(double slope, double q) {
    m_slope = slope;
    m_q = q;
  }*/
  Line(double slope, double q) : m_slope{slope}, m_q{q} {}
};

#endif
