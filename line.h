#ifndef LINE_H
#define LINE_H

class line {
 private:
  double m_slope{};
  double m_q{};

 public:
  double getSlope() const { return m_slope; }
  double setSlope();

  double getQ() const { return m_q; }
  double setQ();

  /*line(double slope, double q) {
    m_slope = slope;
    m_q = q;
  }*/
  line(double slope, double q) : m_slope{slope}, m_q{q} {}
};

#endif
