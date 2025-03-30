#pragma once

#include <Arduino.h>
#include "drivers/DcMotor.h"
#include "drivers/MultiWire.h"
#include "drivers/Gyro.h"
#include "drivers/ColorSensor.h"
#include "drivers/DistanceSensor.h"
#include "drivers/Button.h"

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

DistanceSensor forwardDistanceSensor(4, 5);
DistanceSensor leftDistanceSensor(6, 7);
DistanceSensor rightDistanceSensor(8, 9);

BNO055Gyro gyro(&hardwareWire);

Button startButton(10);

void devicesBegin(){
    hardwareWire.begin();
    softwareWire.begin();

    dcExpansion2.begin();
    dcExpansion3.begin();

    separatorColorSensor.begin();
    clampColorSenor.begin();

    forwardDistanceSensor.begin();
    leftDistanceSensor.begin();
    rightDistanceSensor.begin();

    startButton.begin();

    gyro.begin();

    leftMotor.begin();
    brushMotor.begin();
    separatorMotor.begin();
    rightMotor.begin();
}