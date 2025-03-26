#include <Arduino.h>
#include <Wire.h>
#include "Devices.h"
#include "MultiWire.h"

void setup() {
  Wire.begin();
  Wire.setClock(400000);

  devicesInit();
}

void loop() {

}