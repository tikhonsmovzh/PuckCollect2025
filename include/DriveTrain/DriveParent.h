#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/Sgn.h"

class DriveSample{
protected:

    void Drive(float forward, float turn){
        leftMotor.setPower(forward + turn);
        rightMotor.setPower(forward - turn);
    }

    float GetOriantation(){
        return chopDegrees(gyro.getOrientation().x);
    }

    void encoderReset(){
        rightMotor.softwareEncoderReset();
        leftMotor.softwareEncoderReset();
    }

    void dropProcess(){
        Drive(0.0, 0.0);
        encoderReset();
    }
    PDRegulator *PDreg;
    
public:
    float arg;

    DriveSample(){}; // для лчереди

    DriveSample(PDRegulator &PDr, float t_arg){
        PDreg = &PDr;
        arg = t_arg;
    }

    virtual void Start(){
        encoderReset();
        PDreg->start();
    }

    virtual bool Execute(){
        return true;
    }
};