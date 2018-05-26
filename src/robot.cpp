#include <thread>
#include <chrono>
#include "robot.h"
using namespace ev3dev;

class robot
{
public:
  robot(touch_sensor *touch_sensor,
        ultrasonic_sensor *ultrasonic_sensor,
        motor *motor_left,
        motor *motor_right,
        motor *motor_head)
  {
    this.touch_sensor = touch_sensor;
    this.ultrasonic_sensor = ultrasonic_sensor;
    this.motor_left = motor_left;
    this.motor_right = motor_right;
    this.motor_head = motor_head;
    this.motor_left.set_position(0);
    this.motor_right.set_position(0);
    this.motor_head.set_position(-90);
    this.motor_head.set_duty_cycle_sp(40);
    this.motor_left.set_duty_cycle_sp(20);
    this.motor_right.set_duty_cycle_sp(20);
  }
  bool touched()
  {
    return touch_sensor->pressed();
  }
  void sweep180()
  {
    if (motor_head->position() <= -87)
      motor_head->set_position_sp(90).run_to_abs_pos();
    if (motor_head->position() >= 87)
      motor_head->set_position_sp(-90).run_to_abs_pos();
  }
  void turn(int angle)
  {
    motor_left->set_position_sp(angle).run_to_rel_pos();
    motor_right->set_position_sp(-angle).run_to_rel_pos();
    while (motor_left->state().count("running") || motor_right->state().count("running"))
      this_thread::sleep_for(chrono::milliseconds(10));
  }
  void forward()
  {
    motor_left->run_direct();
    motor_right->run_direct();
  }
  void stop()
  {
    motor_left.stop();
    motor_right.stop();
  }
  void stop_sweep()
  {
    motor_head->stop();
  }
  void on()
  {
    motor_head->set_position_sp(-90).run_to_abs_pos();
  }
  void off()
  {
    stop();
    stop_sweep();
  }
  void reset_sweep()
  {
    motor_head->set_position_sp(0).run_to_abs_pos();
  }
  float distance()
  {
    return ultrasonic_sensor->distance_centimeters();
  }
  int angle()
  {
    return motor_head->position();
  }
private:
  touch_sensor *touch_sensor;
  ultrasonic_sensor *ultrasonic_sensor;
  motor *motor_left;
  motor *motor_right;
  motor *motor_head;
}
