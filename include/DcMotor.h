#pragma once

#include <Arduino.h>
#include <Wire.h>

#define MOTOR_BRAKE_MOD 0x7D
#define MOTOR_SET_POWER_C1 0x40
#define MOTOR_SET_POWER_C2 0x41

#define EXPANSION_RESET 0x27
#define EXPANSION_ENABLE 0x25
#define EXPANSION_RESET_TIME 1000

#define REQUEST_MOTOR_CURRENT_C1 0x54
#define REQUEST_MOTOR_CURRENT_C2 0x55

#define REQUEST_MOTOR_POSITION_C1 0x49
#define REQUEST_MOTOR_POSITION_C2 0x4A

#define MOTOR_POSITION_RESET_C1 0x4C
#define MOTOR_POSITION_RESET_C2 0x4D

enum ZeroPowerBehavior
{
    FLOAT = false,
    BRAKE = true
};

enum Direction
{
    FORWARD = false,
    REVERSE = true
};

class DcExpansion
{
private:
    bool _enabled;

    uint32_t _resetTime = 0;

public:
    DcExpansion(uint8_t addr): address(addr) {
        _enabled = false;
    }

    const uint8_t address;

    void init(){
        Wire.beginTransmission(address);
        Wire.write(EXPANSION_RESET);
        Wire.endTransmission();

        _resetTime = millis();
    }

    void enable(){
        if(_enabled)
            return;

        while (millis() - _resetTime < EXPANSION_RESET_TIME);
        
        Wire.beginTransmission(address);
        Wire.write(EXPANSION_ENABLE);
        Wire.endTransmission();

        _enabled = true;
    }

    bool isEnabled(){
        return _enabled;
    }
};


class DcMotor
{
    DcExpansion *_expansion;

    uint8_t _channel;

    bool _direction, _zeroPowerBehavior;

    int8_t _lastPower = 0;

public:
    DcMotor(DcExpansion *expansion, uint8_t channel, bool zeroPowerBehavior = true, bool direction = false)
    {
        _expansion = expansion;
        _channel = channel;
        _zeroPowerBehavior = zeroPowerBehavior;
        _direction = direction;
    }

    void init(){
        if(!_expansion->isEnabled())
            _expansion->enable();

        resetEncoder();
    }

    void setZeroPowerBehavior(bool behavior)
    {
        _zeroPowerBehavior = behavior;
    }

    void setDirection(bool direction)
    {
        _direction = direction;
    }

    void setPower(float power)
    {
        int8_t intPower = min(100.0f, max(-100.0f, power * (_direction ? -100.0f : 100.0f)));

        if (intPower == 0 && _zeroPowerBehavior)
            intPower = MOTOR_BRAKE_MOD;

        if (intPower != _lastPower)
        {
            _lastPower = intPower;

            Wire.beginTransmission(_expansion->address);

            if (_channel == 1)
                Wire.write(MOTOR_SET_POWER_C1);
            else
                Wire.write(MOTOR_SET_POWER_C2);

            Wire.write((uint8_t)intPower);
            Wire.endTransmission();
        }
    }

    float getCurrent(){
        Wire.beginTransmission(_expansion->address);

        if(_channel == 1)
            Wire.write(REQUEST_MOTOR_CURRENT_C1);
        else
            Wire.write(REQUEST_MOTOR_CURRENT_C2);

        Wire.endTransmission();

        Wire.requestFrom(_expansion->address, (uint8_t)2);
        byte buf[2];
        Wire.readBytes(buf, 2);

        return ((int16_t)(buf[0] * 256 + buf[1]) * (_direction ? -1 : 1)) / 1000.0f;
    }

    int32_t getCurrentPosition(){
        Wire.beginTransmission(_expansion->address);

        if(_channel == 1)
            Wire.write(REQUEST_MOTOR_POSITION_C1);
        else
            Wire.write(REQUEST_MOTOR_POSITION_C2);

        Wire.endTransmission();

        Wire.requestFrom(_expansion->address, (uint8_t)4);
        byte buf[4];
        Wire.readBytes(buf, 4);

        uint32_t ticks = buf[0];
        ticks = (ticks * 256) + buf[1];
        ticks = (ticks * 256) + buf[2];
        ticks = (ticks * 256) + buf[3];

        return (int32_t)(ticks) * (_direction ? -1 : 1);
    }

    void resetEncoder(){
        Wire.beginTransmission(_expansion->address);

        if(_channel == 1)
            Wire.write(MOTOR_POSITION_RESET_C1);
        else
            Wire.write(MOTOR_POSITION_RESET_C2);

        Wire.endTransmission();
    }
};