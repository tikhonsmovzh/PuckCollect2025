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
    DriveOnEncoder,     // +
    TurnLocal,          // -
    TurnGlobal          // +
};
*/



class DriveForwardToTheLimit : public DriveSample{
public:
    DriveForwardToTheLimit(PDRegulator &PDr){
        PDreg = &PDr;
    }

    bool Execute() override{ // энкодеры сбрасываются, все норм. ПД тоже сбрасывается
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

    bool Execute() override{
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

    bool Execute() override{
        if (forwardDistanceSensor.readDistance() > arg){
            float errValue = rightDistanceSensor.readDistance() - arg;
            Drive(ROBOT_SPEED, PDreg->update(errValue));
            return false;
        }
        return true;
    }
};

PDRegulator _DATW_PD(0.1f, 0.1f); // надо норм каэфициенты!
DrivingAlongTheWall DrivingAlongTheWallObj(_DATW_PD);


class TravelByEncoderValue : public DriveSample{
public:
    TravelByEncoderValue(PDRegulator &PDr){
        PDreg = &PDr;
    }

    bool Execute() override{
        if (((leftMotor.getCurrentPosition() + rightMotor.getCurrentPosition()) / 2) - arg > 0){
            Drive(ROBOT_SPEED, PDreg->update(rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
            return false;
        }
        return true;
    }
};

PDRegulator _TBEV_PD(0.1f, 0.1f); // надо норм каэфициенты!
TravelByEncoderValue TravelByEncoderValueObj(_TBEV_PD);


class TurnByGlobalCoordinates : public DriveSample{
public:
    TurnByGlobalCoordinates(PDRegulator &PDr){
        PDreg = &PDr;
    }

    bool Execute() override{
        if (!IS_GYRO) return true; //просто пропустит

        auto error = arg - chopDegrees(gyro.getOrientation().x);
        if (abs(error) > ANGLE_ERROR)
        {
            Drive(0.0f, ROBOT_SPEED * sgn(error));
            return false;
        }
        return true;
    }
};

PDRegulator _TBGC_PD(0.1f, 0.1f); // надо норм каэфициенты!
TurnByGlobalCoordinates TurnByGlobalCoordinatesObj(_TBGC_PD);


class TurnByLocalCoordinates : public DriveSample{
private: 
    float startCoords;
public:
    void Start() override{
        encoderReset();
        PDreg->start();
        if (IS_GYRO) startCoords = GetOriantation();
        else startCoords = -1.0; // ненужно
    }
    TurnByLocalCoordinates(PDRegulator &PDr){
        PDreg = &PDr;
    }

    bool Execute() override{
        if (IS_GYRO){
            auto error = arg - (startCoords - GetOriantation());
            if (abs(error) > ANGLE_ERROR)
            {
                Drive(0.0f, ROBOT_SPEED * sgn(error));
                return false;
            }
            return true;
        }else{
            auto error = arg - (((leftMotor.getCurrentPosition() - rightMotor.getCurrentPosition()) / SINGLE_ENCODER_STEP * WHEEL_DISTANCE) / (90 * WHEEL_DISTANCE));
            if (abs(error) > ANGLE_ERROR)
            {
                Drive(0.0f, ROBOT_SPEED * sgn(error));
                return false;
            }
            return true;
        }
    }
};

PDRegulator _TBLC_PD(0.1f, 0.1f); // надо норм каэфициенты!
TurnByLocalCoordinates TurnByLocalCoordinatesObj(_TBLC_PD);