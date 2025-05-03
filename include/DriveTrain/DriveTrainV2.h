#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/Queue.h"
#include "utils/Sgn.h"
#include "DriveTrain/DriveParent.h"
#include "Intake.h"


Queue<DriveSample*> myQueue;


/*
enum SimpleActions{
    DriveToWall,        // +
    TurnOnWall,         // +
    DriveAlongWall,     // +
    DriveOnEncoder,     // +
    TurnLocal,          // +
    TurnGlobal          // +
};
*/


class DriveForwardToTheLimit : public DriveSample{
public:
    DriveForwardToTheLimit(PDRegulator &PDr, float i_arg) : DriveSample(PDr, i_arg) {}
    bool Execute() override{ // энкодеры сбрасываются, все норм. ПД тоже сбрасывается
        if (forwardDistanceSensor.readDistance() > arg){
            Drive(ROBOT_SPEED, PDreg->update(rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
            return false;
        }
        return true;
    }
};


class TurnToTheWall : public DriveSample{
public:
    TurnToTheWall(PDRegulator &PDr, float i_arg) : DriveSample(PDr, i_arg) {}
    bool Execute() override{
        if (forwardDistanceSensor.readDistance() < arg){
            Drive(0.0f, -ROBOT_SPEED);
            return false;
        }
        return true;
    }
};


class DrivingAlongTheWall : public DriveSample{
public:
    DrivingAlongTheWall(PDRegulator &PDr, float i_arg) : DriveSample(PDr, i_arg) {}
    bool Execute() override{
        if (forwardDistanceSensor.readDistance() > arg){
            float errValue = rightDistanceSensor.readDistance() - arg;
            Drive(ROBOT_SPEED, PDreg->update(errValue));
            return false;
        }
        return true;
    }
};


class TravelByEncoderValue : public DriveSample{
public:
    TravelByEncoderValue(PDRegulator &PDr, float i_arg) : DriveSample(PDr, i_arg) {}
    bool Execute() override{
        if (((leftMotor.getCurrentPosition() + rightMotor.getCurrentPosition()) / 2) - arg > 0){
            Drive(ROBOT_SPEED, PDreg->update(rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
            return false;
        }
        return true;
    }
};


class TurnByGlobalCoordinates : public DriveSample{
public:
    TurnByGlobalCoordinates(PDRegulator &PDr, float i_arg) : DriveSample(PDr, i_arg) {}
    bool Execute() override{
        if (!IS_GYRO) return true; //просто пропустит

        auto error = arg - GetOriantation();
        if (abs(error) > ANGLE_ERROR)
        {
            Drive(0.0f, ROBOT_SPEED * sgn(error));
            return false;
        }
        return true;
    }
};


class TurnByLocalCoordinates : public DriveSample{
private: 
    float startCoords;
public:
    TurnByLocalCoordinates(PDRegulator &PDr, float i_arg) : DriveSample(PDr, i_arg) {}
    void Start() override{
        encoderReset();
        PDreg->start();
        if (IS_GYRO) startCoords = GetOriantation();
        else startCoords = -1.0; // ненужно
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