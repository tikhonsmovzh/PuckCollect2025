#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/Sgn.h"
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

    
    bool driveToWall(int etalonDistanceK){
        if (forwardDistanceSensor.readDistance() > ETALON_DISTANCE * etalonDistanceK)
        {
            Drive(ROBOT_SPEED, (rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
            return false;
        }
        return true;
    }

    bool turnByGyro(float angle){
        auto a = angle - chopDegrees(gyro.getOrientation().x);
        if (abs(a) > ANGLE_ERROR)
        {
            Drive(0.0f, ROBOT_SPEED * sgn(a));
            return false;
        }
        return true;
    }


    Steps DriveSteps;
    FunnelSteps FunnelStep;
    BaseSteps BaseStep;
    RandomSteps RandomStep;
    PDRegulator *PDreg;
    ElapseTime *ActionTime;
    uint8_t StepsCount;
    float errValue, randomAngle, timeToDo;

public:
    DriveTrain(PDRegulator &PDr, ElapseTime &actionTime)
    {
        DriveSteps = Diagonal;
        FunnelStep = WallRide;
        BaseStep = TurnMinusNinety;
        RandomStep = GenerateAngle;
        PDreg = &PDr;
        ActionTime = &actionTime;
        StepsCount = 1;
        randomAngle = 1.0f;
        timeToDo = 40.0;
    }

    void begin()
    {
    }

    void start()
    {
        PDreg->start();
        leftMotor.resetEncoder();
        rightMotor.resetEncoder();
    }

    void update()
    {
        switch (DriveSteps)
        {
        case Diagonal:
            if (driveToWall(1))
                DriveSteps = Funnel;
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
                    rightMotor.softwareEncoderReset();
                    leftMotor.softwareEncoderReset();
                    StepsCount++;
                }
                if (StepsCount > BASE_STEP_COUNT)
                {
                    DriveSteps = Base;
                }
                break;

            case Turn:
                if (IS_GYRO)
                {
                    if (turnByGyro(90 * StepsCount))
                    {
                        FunnelStep = WallRide;
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
                    }
                }
                break;
            }

            break;

        case Base:
            if (!IS_GYRO)
            {
                DriveSteps = RandomRide;
                break;
            }
            switch (BaseStep)
            {
            case TurnMinusNinety:
                if (turnByGyro(-90))
                {
                    BaseStep = RightDrive;
                }
                break;

            case RightDrive:
                if (driveToWall(1))
                {
                    BaseStep = TurnZero;
                }
                break;

            case TurnZero:
                if (turnByGyro(0.0f))
                {
                    BaseStep = DownDrive;
                }
                break;

            case DownDrive:
                if (driveToWall(1))
                {
                    if (floorColor == ourColor)
                    {
                        DriveSteps = RandomRide;
                    }
                    else
                    {
                        BaseStep = TurnMinusNinety;
                    }
                }
                break;
            }
            break;

        case RandomRide: // вообще телега отдельная, пока что ее не трогаю
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
                        rightMotor.softwareEncoderReset();
                        leftMotor.softwareEncoderReset();
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
                        ActionTime->reset();

                        rightMotor.softwareEncoderReset();
                        leftMotor.softwareEncoderReset();
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

                    rightMotor.softwareEncoderReset();
                    leftMotor.softwareEncoderReset();
                }
                break;
            }
            break;
        }
    }
};

PDRegulator PDreg(0.1f, 0.1f);
ElapseTime actionTime;
DriveTrain driveTrain(PDreg, actionTime);