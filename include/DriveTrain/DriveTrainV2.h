#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/Sgn.h"
#include "DriveTrain/DriveParent.h"
#include "Intake.h"

class DriveForwardToTheLimit : DriveSample{
public:
    DriveForwardToTheLimit(PDRegulator &PDr, ElapseTime &actionTime){
        PDreg = &PDr;
        PDreg->start();
        ActionTime = &actionTime;
    }


    bool Execute(float Dist){
        if (driveToWall(Dist)){
                return false;
        }
        dropProcess();
        return true;
    }
};

PDRegulator _DFTTL_PD(0.1f, 0.1f);
ElapseTime _DFTTL_T;
DriveForwardToTheLimit DriveForwardToTheLimitObj(_DFTTL_PD, _DFTTL_T);



class TurnToTheWall : DriveSample{
public:
    DriveForwardToTheLimit(PDRegulator &PDr, ElapseTime &actionTime){
        PDreg = &PDr;
        PDreg->start();
        ActionTime = &actionTime;
    }


    bool Execute(float Dist){
        if (forwardDistanceSensor.readDistance() < Dist){
            Drive(0.0f, -ROBOT_SPEED);
            return false;
        }
        dropProcess();
        return true;
    }
}

PDRegulator _TTTW_PD(0.1f, 0.1f);
ElapseTime _TTTW_T;
TurnToTheWall TurnToTheWallObj(_TTTW_PD, _TTTW_T);


class DrivingAlongTheWall : DriveSample{
public:
    DriveForwardToTheLimit(PDRegulator &PDr, ElapseTime &actionTime){
        PDreg = &PDr;
        PDreg->start();
        ActionTime = &actionTime;
    }


    bool Execute(float Dist){
        if (forwardDistanceSensor.readDistance() > Dist){
            errValue = rightDistanceSensor.readDistance() - Dist;
            Drive(ROBOT_SPEED, PDreg->update(errValue));
            return false;
        }
        dropProcess();
        return true;
    }
}

PDRegulator _DATW_PD(0.1f, 0.1f);
ElapseTime _DATW_T;
TurnToTheWall TurnToTheWallObj(_DATW_PD, _DATW_T);