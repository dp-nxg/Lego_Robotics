//============================================================================
// Name        : object_follwing_lego_robot.cpp
// Author      : Durga Prasad Babu Nasika
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : Object Following Lego Robot in C++, Ansi-style
//============================================================================

#include <thread>
#include <chrono>
#include "ev3dev.h"
#include <iostream>
#include <unistd.h>
using namespace std;
using namespace ev3dev;
/*
 * create the objects of the sensors and motors
 */
touch_sensor touch(INPUT_1);
ultrasonic_sensor ultrasonic(INPUT_4);
motor motor_left(OUTPUT_A, motor::motor_large);
motor motor_right(OUTPUT_B, motor::motor_large);
motor motor_head(OUTPUT_D, motor::motor_medium);
/*
 * Some Robot function declarations
 */
void turn(int direction);
void initialize_speeds();
void initialize_positions();
void sweep180();
/*
 * turn the motors to the specific angle
 */
void turn(int angle){
	motor_left. set_position_sp( angle).run_to_rel_pos();
	motor_right.set_position_sp(-angle).run_to_rel_pos();

	while (motor_left.state().count("running") || motor_right.state().count("running"))
		this_thread::sleep_for(chrono::milliseconds(10));
}
/*
 * initilaise the motor angles i.e positions
 */
void initialize_positions(){
	motor_left.set_position(0);
	motor_right.set_position(0);
	motor_head.set_position(-90);
}
/*
 * set the speeds
 */
void initialize_speeds(){
	motor_head.set_duty_cycle_sp(40);
	motor_left.set_duty_cycle_sp(20);
	motor_right.set_duty_cycle_sp(20);
}
/*
 * sweep the head ot 180 degrees -90 to +90
 */
void sweep180(){
	if(motor_head.position()<=-87){
		motor_head.set_position_sp(90).run_to_abs_pos();
	}
	if(motor_head.position()>=87){
		motor_head.set_position_sp(-90).run_to_abs_pos();
	}
}
/*
 * move the robot straight forward
 */
void move(){
	motor_left.run_direct();
	motor_right.run_direct();
}
/*
 * stop the motors ,left and right motors
 */
void stop(){
	motor_left.stop();
	motor_right.stop();
}
/*
 * main method
 */
int main()
{
	/*
	 * state variables that are used for state machine logic
	 */
	bool robot_active_state=false;
	bool toggle_state=false;
	bool object_detected_state=true;
	initialize_positions();
	initialize_speeds();
	/*
	 * A continuous loop
	 */
	while(true){
		this_thread::sleep_for(chrono::milliseconds(200));
		std::cout << "ON: " << robot_active_state << endl;
		/*
		 * touch sensor checking ,acts as on off button for every alternative press, press the button slowely and release when doing this
		 */
		if(touch.is_pressed() && robot_active_state){
			robot_active_state = false;
			stop();
			motor_head.stop();
		}
		else if(touch.is_pressed() && robot_active_state== false)
		{
			robot_active_state = true;
			motor_head.set_position_sp(-90).run_to_abs_pos();
		}
		if(robot_active_state)
		{
			std::cout << "OFF " << endl;
			/*
			 * if robot detected object which is far away  --> stop movement, and turn the head check for the objects in the 180 sweep range
			 */
			if(ultrasonic.distance_centimeters()>40){
				if(toggle_state==true){
					motor_head.set_position_sp(-90).run_to_abs_pos();
					toggle_state=false;
				}
				stop();
				object_detected_state=true;
				sweep180();
			}
			/*
			 *if robot detected some object --> turn to the object and make small movement towards the detected object
			 */
			if(ultrasonic.distance_centimeters()<40 && ultrasonic.distance_centimeters()>10 && object_detected_state == true){
				object_detected_state=false;
				std::cout << "Distance to the object is: : " << ultrasonic.distance_centimeters() << endl;
				toggle_state=true;
				motor_head.stop();
				turn(motor_head.position());
				motor_head.set_position_sp(0).run_to_abs_pos();
				move();
			}
			/*
			 *if robot too close to the detected object --> stop movements of all the motors and head
			 */
			if(ultrasonic.distance_centimeters()<10){
				motor_head.stop();
				stop();
			}
		}
	}
	return 0;
}
