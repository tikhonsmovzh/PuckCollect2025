#pragma once
#include <Arduino.h>

#define IS_GYRO false
#define IS_DISTANCE_SENSOR true
#define ROBOT_SPEED 0.3
#define ANGLE_ERROR 2.5
#define SINGLE_ENCODER_STEP (20 * 24)
#define SINGLE_WHEEL_STEP_BY_ENCODER_TIC (PI * 100) / SINGLE_ENCODER_STEP
#define TURN_45_DEGREE_DISTANCE ((2 * sqrt(288) * PI) / 360) * 45

//тута конфиги надо делать
//#define четотам начетотам