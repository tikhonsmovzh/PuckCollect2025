#include <Arduino.h>
#include "Devices.h"
#include "DriveTrain.h"

Robot robotConfig = Robot(
  leftMotor, 
  rightMotor,
  gyro,
  leftDistanceSensor,
  rightDistanceSensor,
  forwardDistanceSensor); // тих, втащи потом за кривость кода, но поеа что так тк фактически, пишу на плюсах впервые, сори >_<

Driver driveOperator(robotConfig);

void setup() {
  Serial.begin(115200);

  devicesBegin();

  Robot robotConfig = Robot(
    leftMotor, 
    rightMotor,
    gyro,
    leftDistanceSensor,
    rightDistanceSensor,
    forwardDistanceSensor); // тих, втащи потом за кривость кода, но поеа что так тк фактически, пишу на плюсах впервые, сори >_<

  Driver driveOperator = Driver(robotConfig);

  while (!startButton.readState());
  while (startButton.readState());
  
  gyro.reset();
  
  while (!startButton.readState());
  while (startButton.readState());
}

void loop() {
  driveOperator.update();
}