#pragma once

#include <Arduino.h>
#include "drivers/DcMotor.h"
#include "drivers/MultiWire.h"
#include "drivers/Gyro.h"
#include "drivers/ColorSensor.h"

HardwareWire hardwareWire;
SoftwareWire softwareWire(2, 3);

DcExpansion dcExpansion2(2, &hardwareWire);
DcExpansion dcExpansion3(3, &hardwareWire);

DcMotor leftMotor(&dcExpansion2, 1);
DcMotor rightMotor(&dcExpansion2, 2);

DcMotor brushMotor(&dcExpansion3, 1);
DcMotor separatorMotor(&dcExpansion3, 2);

TCS34725ColorSensor separatorColorSensor(&hardwareWire);
TCS34725ColorSensor clampColorSenor(&softwareWire);

void devicesBegin(){
    hardwareWire.begin();
    softwareWire.begin();

    dcExpansion2.begin();
    dcExpansion3.begin();
    dcExpansion2.enable();
    dcExpansion3.enable();

    leftMotor.begin();
    rightMotor.begin();
    brushMotor.begin();
    separatorMotor.begin();

    separatorColorSensor.begin();
    clampColorSenor.begin();
}