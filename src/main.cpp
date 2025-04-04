#include <Arduino.h>
#include "Devices.h"
#include "DriveTrain.h"

void setup() {
  Serial.begin(115200);

  devicesBegin();

  while (!startButton.readState());
  while (startButton.readState());
  
  gyro.reset();
  
  while (!startButton.readState());
  while (startButton.readState());
}

Driver robot;

void loop() {
  robot.update();
}