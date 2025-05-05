#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/Sgn.h"
#include "utils/ElapsedTime.h"

class DriveSample{
protected:

    void Drive(float forward, float turn){
        leftMotor.setPower(forward + turn);
        rightMotor.setPower(forward - turn);
    }

    float GetOriantation(){
        return gyro.getOrientation().x;
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
    ElapseTime *TimeReader;

    
public:
    DriveSample(){}; // для очереди

    DriveSample(PDRegulator &PDr, ElapseTime &Time){
        PDreg = &PDr;
        TimeReader = &Time;
    }

    virtual void Start(){
        encoderReset();
        PDreg->start();
        TimeReader->reset();
    }

    virtual bool Execute(){
        return true;
    }

    bool CheckTime(){
        return (EXECUTION_LIMIT - TimeReader->seconds()) < MAX_TIME_ERROR;
    }
};