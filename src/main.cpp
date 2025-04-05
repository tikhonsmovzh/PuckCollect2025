#include <Arduino.h>
#include "Devices.h"
#include "DriveTrain.h"
#include "Intake.h"

void setup() {
  Serial.begin(115200);

  devicesBegin();
  driveTrain.begin();
  intakeBegin();

  while (!startButton.readState());
  while (startButton.readState());
  
  gyro.reset();

  detectFloor();
  
  while (!startButton.readState());
  while (startButton.readState());

  driveTrain.start();
  intakeStart();
}

void loop() {
  driveTrain.update();
  intakeUpdate();
}