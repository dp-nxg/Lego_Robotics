#include "ev3dev.h"
using namespace ev3dev;

class robot
{
public:
  robot(touch_sensor *touch_sensor,
        ultrasonic_sensor *ultrasonic_sensor,
        motor *motor_left,
        motor *motor_right,
        motor *motor_head);
  void on();
  bool touched();
  void turn(int angle);
  void forward();
  void stop();
  void stop_sweep();
  void off();
  void sweep180();
  float distance();
  int angle();
}
