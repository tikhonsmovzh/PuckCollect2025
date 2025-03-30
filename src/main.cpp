#include <Arduino.h>
#include "Devices.h"

void setup() {
  Serial.begin(115200);

  devicesBegin();

  while (!startButton.readState());
  while (startButton.readState());
  
  gyro.reset();
  
  while (!startButton.readState());
  while (startButton.readState());
}

void loop() {

}