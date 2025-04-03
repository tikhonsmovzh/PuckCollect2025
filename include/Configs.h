#pragma once

#define IS_GYRO false
#define IS_DISTANCE_SENSOR true
#define SINGLE_ENCODER_STEP 20 * 24
#define SINGLE_WHEEL_STEP SINGLE_ENCODER_STEP //умножить на длиину колеса когда ответят


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