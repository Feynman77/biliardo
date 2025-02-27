#include "ball.h"
#include "setting.h"
class system {
 private:
  Ball m_ball{};
  Setting m_setting{};

 public:
  Ball getBall() const { return m_ball; }
  void setBall();

  Setting getSetting() const { return m_setting; }
  void setSetting();
};