#include <Arduino.h>
#include "Devices.h"
#include "DriveTrain/Controller.h"
#include "DriveTrain/DriveTrainV2.h"
#include "Intake.h"

void setup() {
  Serial.begin(115200);

  devicesBegin();

  while (!startButton.readState());
  while (startButton.readState());
  
  intakeBegin();
  
  gyro.reset();

  detectFloor();
  
  while (!startButton.readState());
  while (startButton.readState());

  intakeStart();

  Queue<SimpleAction> q;
}

void loop() {
  BodyControl(q);
  intakeUpdate();
}