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
PDRegulator PDreg(1.0f, 1.0f);
Driver robot(PDreg);

void loop() {
  robot.update();
}