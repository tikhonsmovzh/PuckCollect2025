#include <Arduino.h>
#include "Devices.h"
// #include "DriveTrain/DriveTrain.h"
//#include "Intake.h"

void setup() {
  Serial.begin(115200);

  devicesBegin();

  // while (!startButton.readState());
  // while (startButton.readState());
  
  // intakeBegin();
  
  // gyro.reset();

  // detectFloor();

  // driveTrainBegin();
  
  // while (!startButton.readState());
  // while (startButton.readState());

  // intakeStart();
  // driveTrainStart();
}

void loop() {
  // driveTrainUpdate();
  // intakeUpdate();
  Serial.println(rightDistanceSensor.readDistance());
  delay(15);
}