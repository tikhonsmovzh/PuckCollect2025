#pragma once

#include <Arduino.h>
#include "drivers/DcMotor.h"
#include "drivers/MultiWire.h"
#include "drivers/Gyro.h"
#include "drivers/ColorSensor.h"

HardwareWire hardwareWire = HardwareWire();
DcExpansion dcExpansion2(2, &hardwareWire);
DcExpansion dcexpansion3(3, &hardwareWire);

void devicesBegin(){
    hardwareWire.begin();

    dcExpansion2.begin();
    dcexpansion3.begin();
    dcExpansion2.enable();
    dcexpansion3.enable();
}