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

/*       это тупо рефы для меня
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
*/
    PDRegulator *PDreg;
    
public:
    float arg;

    virtual void Start(){
        encoderReset();
        PDreg->start();
    }

    virtual bool Execute(){
        return true;
    }
};