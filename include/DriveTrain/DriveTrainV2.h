#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/Queue.h"
#include "utils/Sgn.h"
#include "DriveTrain/DriveParent.h"
#include "Intake.h"


Queue<DriveSample> myQueue;


/*
enum SimpleActions{
    DriveToWall,        // +
    TurnOnWall,         // +
    DriveAlongWall,     // +
    DriveOnEncoder,     // -
    TurnLocal,          // -
    TurnGlobal          // -
};
*/



class DriveForwardToTheLimit : public DriveSample{
public:
    DriveForwardToTheLimit(PDRegulator &PDr){
        PDreg = &PDr;
    }

    bool Execute(){ // энкодеры сбрасываются, все норм. ПД тоже сбрасывается
        if (forwardDistanceSensor.readDistance() > arg){
            Drive(ROBOT_SPEED, PDreg->update(rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
            return false;
        }
        return true;
    }
};

PDRegulator _DFTTL_PD(0.1f, 0.1f); // надо норм каэфициенты!
DriveForwardToTheLimit DriveForwardToTheLimitObj(_DFTTL_PD);



class TurnToTheWall : public DriveSample{
public:
    TurnToTheWall(PDRegulator &PDr){
        PDreg = &PDr;
    }

    bool Execute(){
        if (forwardDistanceSensor.readDistance() < arg){
            Drive(0.0f, -ROBOT_SPEED);
            return false;
        }
        return true;
    }
};

PDRegulator _TTTW_PD(0.1f, 0.1f); // надо норм каэфициенты!
TurnToTheWall TurnToTheWallObj(_TTTW_PD);


class DrivingAlongTheWall : public DriveSample{
public:
    DrivingAlongTheWall(PDRegulator &PDr){
        PDreg = &PDr;
    }

    bool Execute(){
        if (forwardDistanceSensor.readDistance() > arg){
            float errValue = rightDistanceSensor.readDistance() - arg;
            Drive(ROBOT_SPEED, PDreg->update(errValue));
            return false;
        }
        return true;
    }
};

PDRegulator _DATW_PD(0.1f, 0.1f); // надо норм каэфициенты!
DrivingAlongTheWall DrivingAlongTheWall(_DATW_PD);


class TravelByEncoderValue : public DriveSample{
public:
    TravelByEncoderValue(PDRegulator &PDr){
        PDreg = &PDr;
    }

    bool Execute(){
        if (((leftMotor.getCurrentPosition() + rightMotor.getCurrentPosition()) / 2) - arg > 0){
            Drive(ROBOT_SPEED, PDreg->update(rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
            return false;
        }
        return true;        
    }
};