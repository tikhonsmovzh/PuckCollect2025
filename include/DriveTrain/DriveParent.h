#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/Sgn.h"

class DriveSample{
protected:
    void startTimer(){
        ActionTime->reset();
    }

    bool checkTimer(){
        return timeToExecute - ActionTime->seconds() < 0.5;
    }

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

    bool driveToWall(float dist){
        
        if (forwardDistanceSensor.readDistance() > dist)
        {
            Drive(ROBOT_SPEED, PDreg->update(rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
            return false;
        }
        encoderReset();
        return true;
    }

    bool turnByGyro(float angle){
        auto error = angle - chopDegrees(gyro.getOrientation().x);
        if (abs(error) > ANGLE_ERROR)
        {
            Drive(0.0f, ROBOT_SPEED * sgn(error));
            return false;
        }
        return true;
    }
    void dropProcess(){
        Drive(0.0, 0.0);
        encoderReset();
    }

    PDRegulator *PDreg;
    ElapseTime *ActionTime;
    bool compliteTask;
    float timeToExecute;
};