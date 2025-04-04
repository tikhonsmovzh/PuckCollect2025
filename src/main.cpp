#include <Arduino.h>
#include "Devices.h"
#include "DriveTrain.h"

void setup() {
  Serial.begin(115200);

  devicesBegin();
  driveTrain.begin();

  while (!startButton.readState());
  while (startButton.readState());
  
  gyro.reset();
  
  while (!startButton.readState());
  while (startButton.readState());

  driveTrain.start();
}

void loop() {
  driveTrain.update();
}