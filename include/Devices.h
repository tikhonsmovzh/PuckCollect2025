#pragma once

#include <Arduino.h>
#include "DcMotor.h"

DcExpansion dcExpansion1(2);
DcExpansion dcExpansion2(3);

DcMotor leftMotor(&dcExpansion1, 1);
DcMotor rightMotor(&dcExpansion1, 2);
DcMotor brushMotor(&dcExpansion2, 1);
DcMotor separatorMotor(&dcExpansion2, 2);

void devicesInit(){
    dcExpansion1.init();
    dcExpansion2.init();

    leftMotor.init();
    rightMotor.init();
    brushMotor.init();
    separatorMotor.init();
}