#include <cinttypes>

#include "ball.h"
#include "setting.h"
class System {
 private:
  Ball m_ball{0,0};
  Setting m_setting{0,0,0};

 public:
  Ball getBall() const { return m_ball; }
  void setBall();

  Setting getSetting() const { return m_setting; }
  void setSetting();

  System(Ball ball, Setting setting) : m_ball{ball}, m_setting{setting} {}
};