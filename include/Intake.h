#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"

PDRegulator _separatorRegulator(SEPARATOR_P, SEPARATOR_D);
int32_t _targetSeparatorPos = 0;

void intakeBegin(){
    separatorMotor.setMaxPower(SEPARATOR_MAX_POWER);
}

void intakeStart(){
    clampServo.write(CLAMP_SERVO_CALMP_POS);
    _regulator.start();
}

void intakeUpdate(){
    separatorMotor.setPower(_separatorRegulator.update(_targetSeparatorPos - separatorMotor.getCurrentPosition()));

    Color clampColor = clampColorSenor.readRawRGB();
    Color separatorColor = separatorColorSensor.readRawRGB();
}
