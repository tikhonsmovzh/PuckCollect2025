#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/Queue.h"
#include "utils/Sgn.h"
#include "DriveTrain/DriveSample.h"
#include "Intake.h"

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
    float _distance;
public:
    DriveForwardToTheLimit(PDRegulator &PDr, float distance) : DriveSample(PDr) {
        _distance = distance;
    }

    bool Execute() override{ // энкодеры сбрасываются, все норм. ПД тоже сбрасывается
        if (forwardDistanceSensor.readDistance() > _distance){
            Drive(ROBOT_SPEED, PDreg->update(rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
            return false;
        }
        return true;
    }
};


class TurnToTheWall : public DriveSample{
    float _distance;
public:
    TurnToTheWall(PDRegulator &PDr, float distance) : DriveSample(PDr) {
        _distance = distance;
    }

    bool Execute() override{
        if (forwardDistanceSensor.readDistance() < _distance){
            Drive(0.0f, -ROBOT_SPEED);
            return false;
        }
        return true;
    }
};


class DrivingAlongTheWall : public DriveSample{
    float _distance;
public:
    DrivingAlongTheWall(PDRegulator &PDr, float distance) : DriveSample(PDr) {
        _distance = distance;
    }
    bool Execute() override{
        if (forwardDistanceSensor.readDistance() > _distance){
            float errValue = rightDistanceSensor.readDistance() - _distance;
            Drive(ROBOT_SPEED, PDreg->update(errValue));
            return false;
        }
        return true;
    }
};


class TravelByEncoderValue : public DriveSample{
    float _encPos;
public:
    TravelByEncoderValue(PDRegulator &PDr, float encPos) : DriveSample(PDr) {
        _encPos = encPos;
    }

    bool Execute() override{
        if (((leftMotor.getCurrentPosition() + rightMotor.getCurrentPosition()) / 2) > _encPos){
            Drive(-ROBOT_SPEED, PDreg->update(rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
            return false;
        }
        return true;
    }
};


class TurnByGlobalCoordinates : public DriveSample{
    float _targetRotate;
public:
    TurnByGlobalCoordinates(PDRegulator &PDr, float targetRotate) : DriveSample(PDr) {
        _targetRotate = targetRotate;
    }
    bool Execute() override{
        if (!IS_GYRO) return true; //просто пропустит

        auto error = chopDegrees(_targetRotate - GetOriantation());
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
    float _targetTurn;
    float startCoords;
public:
    TurnByLocalCoordinates(PDRegulator &PDr, float targetTurn) : DriveSample(PDr) {
        _targetTurn = targetTurn;
    }
    void Start() override{
        DriveSample::Start();
        if (IS_GYRO) startCoords = GetOriantation();
    }

    bool Execute() override{
        if (IS_GYRO){
            auto error = chopDegrees(_targetTurn - (startCoords - GetOriantation()));
            if (abs(error) > ANGLE_ERROR)
            {
                Drive(0.0f, ROBOT_SPEED * sgn(error));
                return false;
            }
            return true;
        }else{
            auto error = chopDegrees(_targetTurn - chopDegrees(((leftMotor.getCurrentPosition() - rightMotor.getCurrentPosition()) / SINGLE_ENCODER_STEP * WHEEL_DISTANCE) / (90 * WHEEL_DISTANCE)));
            if (abs(error) > ANGLE_ERROR)
            {
                Drive(0.0f, ROBOT_SPEED * sgn(error));
                return false;
            }
            return true;
        }
    }
};