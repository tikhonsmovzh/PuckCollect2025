#include <Arduino.h>
#include "drivers/DcMotor.h"
#include "drivers/MultiWire.h"
#include "Devices.h"

void setup() {
  Serial.begin(9600);

  devicesBegin();
}

void loop() {

}