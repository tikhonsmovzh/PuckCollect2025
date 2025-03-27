#pragma once

#include <Arduino.h>
#include "drivers/DcMotor.h"
#include "drivers/MultiWire.h"
#include "drivers/Gyro.h"

HardwareWire hardwareWire;

DcExpansion dcExpansion1(2, &hardwareWire);
DcExpansion dcExpansion2(3, &hardwareWire);

DcMotor leftMotor(&dcExpansion1, 1);
DcMotor rightMotor(&dcExpansion1, 2);
DcMotor brushMotor(&dcExpansion2, 1);
DcMotor separatorMotor(&dcExpansion2, 2);

void devicesBegin(){
    hardwareWire.begin();
    hardwareWire.setClock(400000);

    dcExpansion1.begin();
    dcExpansion2.begin();

    leftMotor.begin();
    rightMotor.begin();
    brushMotor.begin();
    separatorMotor.begin();
}