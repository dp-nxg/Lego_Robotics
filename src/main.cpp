#include "ev3dev.h"
#include "robot.h"
using namespace std;
using namespace ev3dev;

int main()
{
  robot robot(&touch_sensor(INPUT_1), &ultrasonic_sensor(INPUT_4), &motor(OUTPUT_A, motor::motor_large), &motor(OUTPUT_B, motor::motor_large), &motor(OUTPUT_D, motor::motor_medium));
  follower follower(&robot);
  while (true)
    follower.step();
  return 0;
}
