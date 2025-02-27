#ifndef SETTING_H
#define SETTING_H

class Setting {
 private:
  double m_l{};
  double m_r_1{};
  double m_r_2{};

 public:
  double getL() const { return m_l; }
  void setL();

  double getR_1() const { return m_r_1; }
  void setR_1();

  double getR_2() const { return m_r_2; }
  void setR_2();
};

#endif

//possiamo mettere come variabili anche i due estremi che tanto so fissi e riuscire così a definire le rette dei setting
/*
struct point{
double x;
double y;
};





*/