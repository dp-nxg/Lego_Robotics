#include "follower.h"
using namespace std;

class follower
{
public:
  follower(robot *robot)
  {
    this.robot = robot;
    on = false;
    toggle_state = false;
    object_detected = true;
  }
  void step()
  {
    if (robot->touched())
      _touched();
    else if (on)
      _step();
  }

private:
  robot *robot;
  bool on;
  bool toggle_state;
  bool object_detected;
  void _touched()
  {
    if (on)
      robot->off();
    else
      robot->on();
    on = !on;
  }
  void _step()
  {
    float distance = robot->distance();
    if (distance > 40)
    {
      if (toggle_state)
      {
        robot->on();
        toggle_state = false;
      }
      robot->stop();
      object_detected = true;
      sweep180();
    }
    else if (distance < 40 && distance > 10 && object_detected)
    {
      object_detected = false;
      toggle_state = true;
      robot->stop_sweep();
      robot->turn(robot->angle());
      // motor_head.set_position_sp(0).run_to_abs_pos();
      robot->forward();
    }
    else if (distance < 10)
    {
      robot->off();
    }
  }
}
