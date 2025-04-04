#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"



enum Steps{
    Diagonal,
    Funnel,
    Base
};

class Driver{
private:
    float GetOriantation(){
        return gyro.getOrientation().x; // надо будет свапнуть х на то что будет
    }

    void Drive(float forward, float turn){
        leftMotor.setPower(forward + turn); // спасибо тежан :)
        rightMotor.setPower(forward - turn);
    }

    Steps DriveSteps;
    float junkValues; // почему то, ни одна моя прога без них не обходится 0_о
public:
    Driver(){
        DriveSteps = Diagonal;
        junkValues = 0.0f;
    }

    void update(){
        switch (DriveSteps){
        case Diagonal:
            if (forwardDistanceSensor.readDistance() > SIDE_DISTANCE){
                if (junkValues == 1.0f){
                    Drive(ROBOT_SPEED, 0.0f);
                    junkValues = 0.0f;
                }
            }else{
                Drive(0.0f, 0.0f);
                DriveSteps = Funnel;
            }
            break;
        
        case Funnel:

        case Base:
            break;

        default:
            break;
        }
    }
};