#pragma once
#include <Arduino.h>

#define IS_GYRO false
#define IS_DISTANCE_SENSOR true
#define ROBOT_SPEED 0.3
<<<<<<< Updated upstream
#define ANGLE_ERROR 2.5
#define SINGLE_ENCODER_STEP (20 * 24)
#define SINGLE_WHEEL_STEP_BY_ENCODER_TIC (PI * 100) / SINGLE_ENCODER_STEP
#define TURN_45_DEGREE_DISTANCE ((2 * sqrt(288) * PI) / 360) * 45
=======
#define SINGLE_ENCODER_STEP (20 * 24)
#define SINGLE_WHEEL_STEP_BY_ENCODER_TIC (PI * 100) / SINGLE_ENCODER_STEP
#define TURN_45_DEGREE_DISTANCE = ((2 * sqrt(288) * PI) / 360) * 45
>>>>>>> Stashed changes


#define CLAMP_SERVO_CALMP_POS 0
#define CLAMP_SERVO_UNCLAMP_POS 20

#define SEPARATOR_P 0.1f
#define SEPARATOR_D 0.0f

#define SEPARATOR_MAX_POWER 0.7
#define SEPARATOR_MOTOR_STEP 720 / 3

#define COLOR_SENSOR_CLAMP_SENS 10
#define COLOR_SENSOR_SEPARARTOR_SENS 10

//тута конфиги надо делать
//#define четотам начетотам