#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "Intake.h"

enum Steps
{
    Diagonal,
    Funnel,
    Base,
    RandomRide
};

enum FunnelSteps
{
    WallRide,
    Turn
};

enum BaseSteps
{
    TurnMinusNinety,
    RightDrive,
    TurnZero,
    DownDrive
};

enum RandomSteps
{
    GenerateAngle,
    TurnRAng,
    DriveToWall
};

class DriveTrain
{
private:
    float GetOriantation()
    {
        return gyro.getOrientation().x; // надо будет свапнуть х на то что будет
    }

    void Drive(float forward, float turn)
    {
        leftMotor.setPower(forward + turn); // спасибо тежан :)
        rightMotor.setPower(forward - turn);
    }

    float randomFloat(float min, float max)
    { // это из инета
        return ((float)rand() / RAND_MAX) * (max - min) + min;
    }

    Steps DriveSteps;
    FunnelSteps FunnelStep;
    BaseSteps BaseStep;
    RandomSteps RandomStep;
    PDRegulator *PDreg;
    ElapseTime ActionTime;
    uint8_t StepsCount;
    float errValue, randomAngle, timeToDo;
public:
    DriveTrain(PDRegulator &PDr)
    {
        DriveSteps = Diagonal;
        FunnelStep = WallRide;
        BaseStep = TurnMinusNinety;
        RandomStep = GenerateAngle;
        ActionTime = ElapseTime();
        PDreg = &PDr;
        StepsCount = 1;
        randomAngle = 1.0f;
        timeToDo = 40.0;
        leftMotor.resetEncoder();
        rightMotor.resetEncoder();
    }

    void begin()
    {
    }

    void start()
    {
        PDreg->start();
    }

    void update()
    {
        if (ActionTime.seconds() > timeToDo){
            if (DriveSteps != RandomRide){
                DriveSteps = static_cast<Steps>(static_cast<int>(DriveSteps) + 1); // убей меня :)
            }
        }
        switch (DriveSteps)
        {
        case Diagonal:
            if (forwardDistanceSensor.readDistance() > ETALON_DISTANCE)
            {
                Drive(ROBOT_SPEED, (rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
            }
            else
            {
                DriveSteps = Funnel;
                ActionTime.reset();
            }
            break;

        case Funnel:
            switch (FunnelStep)
            {
            case WallRide:
                errValue = rightDistanceSensor.readDistance() - ETALON_DISTANCE * StepsCount;
                Drive(ROBOT_SPEED, PDreg->update(errValue));
                if (forwardDistanceSensor.readDistance() < ETALON_DISTANCE * StepsCount)
                {
                    FunnelStep = Turn;
                    rightMotor.resetEncoder();
                    leftMotor.resetEncoder();
                    StepsCount++;
                }
                if (StepsCount > BASE_STEP_COUNT)
                {
                    DriveSteps = Base;
                    ActionTime.reset();
                }
                break;

            case Turn:
                if (IS_GYRO)
                {
                    if (abs(chopDegrees(chopDegrees(90 * StepsCount) - gyro.getOrientation().x)) > ANGLE_ERROR)
                    {
                        Drive(0.0f, -ROBOT_SPEED);
                    }
                    else
                    {
                        FunnelStep = WallRide;
                        ActionTime.reset();
                    }
                }
                else
                {
                    if (forwardDistanceSensor.readDistance() < ETALON_DISTANCE * StepsCount)
                    {
                        Drive(0.0f, -ROBOT_SPEED);
                    }
                    else
                    {
                        FunnelStep = WallRide;
                        ActionTime.reset();
                    }
                }
                break;
            }

            break;

        case Base:
            if (!IS_GYRO)
            {
                DriveSteps = RandomRide;
                ActionTime.reset();
                break;
            }
            switch (BaseStep)
            {
            case TurnMinusNinety:
                Drive(0.0f, chopDegrees(-90.0 - gyro.getOrientation().x));
                if (abs(chopDegrees(-90.0f - gyro.getOrientation().x)) < ANGLE_ERROR)
                {
                    BaseStep = RightDrive;
                }
                break;

            case RightDrive:
                if (forwardDistanceSensor.readDistance() > ETALON_DISTANCE)
                {
                    Drive(ROBOT_SPEED, (rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
                }
                else
                {
                    BaseStep = TurnZero;
                }
                break;

            case TurnZero:
                Drive(0.0f, -gyro.getOrientation().x);
                if (abs(gyro.getOrientation().x) < ANGLE_ERROR)
                {
                    BaseStep = DownDrive;

                    rightMotor.resetEncoder();
                    leftMotor.resetEncoder();
                }
                break;

            case DownDrive:
                if (forwardDistanceSensor.readDistance() > ETALON_DISTANCE)
                {
                    Drive(ROBOT_SPEED, (rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
                }
                else
                {
                    if (floorColor == ourColor){
                        DriveSteps = RandomRide;
                        ActionTime.reset();
                    }else{
                        BaseStep = TurnMinusNinety;
                    }
                }
                break;
            }
            break;

        case RandomRide:
            switch (RandomStep)
            {
            case GenerateAngle:
                randomAngle = randomFloat(-180.0, 180.0);
                leftMotor.softwareEncoderReset();
                rightMotor.softwareEncoderReset();
                break;

            case Turn:
                if (IS_GYRO)
                {
                    Drive(0.0f, chopDegrees(randomAngle - gyro.getOrientation().x));
                    if (abs(chopDegrees(randomAngle - gyro.getOrientation().x)) < ANGLE_ERROR)
                    {
                        RandomStep = DriveToWall;
                        rightMotor.resetEncoder();
                        leftMotor.resetEncoder();
                    }
                }
                else
                {
                    Drive(0.0f, -ROBOT_SPEED);
                    if ((leftMotor.getCurrentPosition() + rightMotor.getCurrentPosition()) / 2 < abs(randomAngle))
                    {
                        Drive(0.0f, ROBOT_SPEED);
                    }
                    else
                    {
                        RandomStep = DriveToWall;
                        ActionTime.reset();

                        rightMotor.resetEncoder();
                        leftMotor.resetEncoder();
                    }
                }
                break;

            case DriveToWall:
                if (forwardDistanceSensor.readDistance() > ETALON_DISTANCE)
                {
                    Drive(ROBOT_SPEED, (rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
                }
                else
                {
                    DriveSteps = RandomRide;

                    rightMotor.resetEncoder();
                    leftMotor.resetEncoder();
                }
                break;
            }
            break;
        }
    }
};

PDRegulator PDreg(0.1f, 0.1f);
DriveTrain driveTrain(PDreg);