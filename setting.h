#ifndef SETTING_H
#define SETTING_H

#include "line.h"


class Setting {
 private:
  double m_l{};
  double m_r_1{};
  double m_r_2{};

 public:
  double getL() const { return m_l; }
  void setL(double);

  double getR_1() const { return m_r_1; }
  void setR_1(double);

  double getR_2() const { return m_r_2; }
  void setR_2(double);

  Setting(double l, double r_1, double r_2) : m_l{l}, m_r_1{r_1}, m_r_2{r_2} {}

  Line upLine() {};
  Line downLine() {};
};

#endif

// possiamo mettere come variabili anche i due estremi che tanto so fissi e
// riuscire così a definire le rette dei setting
/*
struct point{
double x;
double y;
};





*/