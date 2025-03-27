#pragma once

#include <Arduino.h>
#include "MultiWire.h"

#define DEFAULT_BNO055_ADDRESS 0x28 //0x29

#define BNO055_CONFIG_MODE 0X00
#define BNO055_OPERATION_MODE 0x0C
#define BNO055_MODE_SWAP 0X3D

#define BNO055_POWER_MODE_SWAP 0X3E
#define BNO055_POWER_MODE_NORMAL 0x00

#define BNO055_PAGE_ID 0x007
#define BNO055_SET_USE_EXTRA_CRYSTAL 0X3F
#define BNO055_USE_EXTRA_CRYSTAL 0x80
#define BNO055_NOT_USE_EXTRA_CRYSTAL 0x00

#define BNO055_SET_UNITS 0X3B

#define BNO055_REQUEST_ORIENTATION 0X1A

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

float chopDegrees(float val){
    float chopedVal = val;

    while (abs(chopedVal) > 180.0f)
        chopedVal -= 360.0f * sgn(chopedVal);
        
    return chopedVal;
}

float chopRadians(float val){
    float chopedVal = val;

    while (abs(chopedVal) > PI)
        chopedVal -= 2.0f * PI * sgn(chopedVal);
        
    return chopedVal;
}

enum AngleUnit{
    RADIANS = 0,
    DEGREES = 1
};

struct Oriantation{
    float x, y, z;

    Oriantation(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Oriantation(){
        x = 0;
        y = 0;
        z = 0;
    }
};

class BNO055Gyro
{
private:
    IWire *_wire;
    uint8_t _address;
    AngleUnit _angleUnit;
    Oriantation _startOrientation;

    Oriantation getRawOrientation(){
        _wire->write8(_address, BNO055_REQUEST_ORIENTATION);

        _wire->requestFrom(_address, 6);
        uint8_t buffer[6];
        _wire->readBytes(buffer, 6);

        int16_t x, y, z;

        x = ((int16_t)buffer[0]) | (((int16_t) buffer[1]) << 8);
        y = ((int16_t)buffer[2]) | (((int16_t) buffer[3]) << 8);
        z = ((int16_t)buffer[4]) | (((int16_t) buffer[5]) << 8);

        return Oriantation(x / 16.0f, y / 16.0f, z / 16.0f);
    }

public:
    BNO055Gyro(IWire *wire, AngleUnit angleUnit = DEGREES, uint8_t address = DEFAULT_BNO055_ADDRESS){
        _wire = wire;
        _address = address;
        _angleUnit = angleUnit;
    }

    void begin(){
        uint8_t unitBit = (_angleUnit == RADIANS ? 1 : 0);
        uint8_t unitData = (1 << 7) | (0 << 4) | (unitBit << 2) | (unitBit << 1) | (0 << 0);

        _wire->beginTransmission(_address);
        
        _wire->write(BNO055_MODE_SWAP);
        _wire->write(BNO055_CONFIG_MODE);

        _wire->write(BNO055_POWER_MODE_SWAP);
        _wire->write(BNO055_POWER_MODE_NORMAL);

        _wire->write(BNO055_PAGE_ID);
        _wire->write(0x0);

        _wire->write(BNO055_SET_UNITS);
        _wire->write(unitData);

        _wire->write(BNO055_SET_USE_EXTRA_CRYSTAL);
        _wire->write(BNO055_USE_EXTRA_CRYSTAL);

        _wire->write(BNO055_MODE_SWAP);
        _wire->write(BNO055_OPERATION_MODE);

        _wire->endTransmission();

        _startOrientation = getRawOrientation();
    }

    void reset(){
        _startOrientation = getRawOrientation();
    }

    Oriantation getOrientation(){
        Oriantation orientation = getRawOrientation();

        orientation.x -= _startOrientation.x;
        orientation.y -= _startOrientation.y;
        orientation.z -= _startOrientation.z;

        if(_angleUnit == RADIANS){
            orientation.x = chopRadians(orientation.x);
            orientation.y = chopRadians(orientation.y);
            orientation.z = chopRadians(orientation.z);
        }
        else{
            orientation.x = chopDegrees(orientation.x);
            orientation.y = chopDegrees(orientation.y);
            orientation.z = chopDegrees(orientation.z);
        }

        return orientation;
    }
};
