#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"



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
    float speedK = 1.0f;
    PDRegulator *PDreg;
    static int junkValuesI;
    static float junkValuesF, errValue; // почему то, ни одна моя прога без них не обходится 0_о
public:
    Driver(PDRegulator& PDr){
        DriveSteps = Diagonal;
        junkValuesF = 0.0f;
        junkValuesI = 2;
        PDreg = &PDr;
    }

    void update(){
        switch (DriveSteps){
        case Diagonal:
            if (forwardDistanceSensor.readDistance() > SIDE_DISTANCE){
                if (junkValuesI == 2){
                    Drive(ROBOT_SPEED, 0.0f);
                    junkValuesI = 1;
                }
            }else{
                Drive(0.0f, 0.0f);
                DriveSteps = Funnel;
            }
            break;
        
        case Funnel:
            errValue = rightDistanceSensor.readDistance() - ETALON_SIDE_DISTANCE;
            Drive(ROBOT_SPEED, (PDreg->update(errValue) * speedK) - junkValuesF);

            if (forwardDistanceSensor.readDistance() < SIDE_DISTANCE){
                junkValuesF = 1.0f;
            }else{
                junkValuesF = 0.0f;
            }
            break;

        case Base:
            break;

        default:
            break;
        }
    }
};