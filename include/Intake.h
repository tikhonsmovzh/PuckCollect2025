#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/ElapsedTime.h"

PDRegulator _separatorRegulator(SEPARATOR_P, SEPARATOR_D);
int32_t _targetSeparatorPos = 0;

enum ColorType
{
    RED,
    BLUE,
    WHITE
};

ColorType floorColor = WHITE, puckColor = WHITE, ourColor = RED;

ElapseTime _clampTimer;

void intakeBegin()
{
    separatorMotor.setMaxPower(SEPARATOR_MAX_POWER);
}

void intakeStart()
{
    clampServo.write(CLAMP_SERVO_CALMP_POS);
    _regulator.start();
    _clampTimer.reset();
}

void detectFloor()
{
    updateColorSensors();
    ourColor = floorColor;
}

void updateColorSensors()
{
    Color clampColor = clampColorSenor.readRawRGB();
    Color separatorColor = separatorColorSensor.readRawRGB();

    if (clampColor.r - max(clampColor.g, clampColor.b) > COLOR_SENSOR_CLAMP_SENS)
        floorColor = RED;
    else if (clampColor.b - max(clampColor.g, clampColor.r) > COLOR_SENSOR_CLAMP_SENS)
        floorColor = BLUE;
    else
        floorColor = WHITE;

    if (floorColor != WHITE)
        puckColor = WHITE;
    else
    {
        if (separatorColor.r - max(separatorColor.g, separatorColor.b) > COLOR_SENSOR_SEPARARTOR_SENS)
            puckColor = RED;
        else if (separatorColor.b - max(separatorColor.g, separatorColor.r) > COLOR_SENSOR_SEPARARTOR_SENS)
            puckColor = BLUE;
        else
            puckColor = WHITE;
    }
}

void intakeUpdate()
{
    updateColorSensors();

    int32_t separatorErr = _targetSeparatorPos - separatorMotor.getCurrentPosition();

    separatorMotor.setPower(_separatorRegulator.update(separatorErr));
}
