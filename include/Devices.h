#pragma once

#include <Arduino.h>
#include "DcMotor.h"
#include "MultiWire.h"

HardwareWire hardwareWire;

DcExpansion dcExpansion1(2, &hardwareWire);
DcExpansion dcExpansion2(3, &hardwareWire);

DcMotor leftMotor(&dcExpansion1, 1);
DcMotor rightMotor(&dcExpansion1, 2);
DcMotor brushMotor(&dcExpansion2, 1);
DcMotor separatorMotor(&dcExpansion2, 2);

void devicesInit(){
    hardwareWire.begin();
    hardwareWire.setClock(400000);

    dcExpansion1.init();
    dcExpansion2.init();

    leftMotor.init();
    rightMotor.init();
    brushMotor.init();
    separatorMotor.init();
}