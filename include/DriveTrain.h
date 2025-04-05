#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"

enum Steps{
    Diagonal,
    Funnel,
    Base,
    RandomRide
};

enum FunnelSteps{
    WallRide,
    Turn
};

enum BaseSteps{
    TurnMinusNinety,
    RightDrive,
    TurnZero,
    DownDrive
};

enum RandomSteps{
    GenerateAngle,
    Turn,
    DriveToWall
};

class DriveTrain{
private:
    float GetOriantation(){
        return gyro.getOrientation().x; // надо будет свапнуть х на то что будет
    }

    void Drive(float forward, float turn){
        leftMotor.setPower(forward + turn); // спасибо тежан :)
        rightMotor.setPower(forward - turn);
    }

    float randomFloat(float min, float max) { // это из инета
        return ((float)rand() / RAND_MAX) * (max - min) + min;
    }

    Steps DriveSteps;
    FunnelSteps FunnelStep;
    BaseSteps BaseStep;
    PDRegulator *PDreg;
    uint8_t StepsCount;
    float errValue, randomAngle; // почему то, ни одна моя прога без них не обходится 0_о
public:
    DriveTrain(PDRegulator& PDr){
        DriveSteps = Diagonal;
        FunnelStep = WallRide;
        BaseStep = TurnMinusNinety;
        PDreg = &PDr;
        StepsCount = 1;
        randomAngle = 1.0f;
        leftMotor.resetEncoder();
        rightMotor.resetEncoder();
    }

    void begin(){

    }

    void start(){

    }

    void update(){
        switch (DriveSteps){
        case Diagonal:
            if (forwardDistanceSensor.readDistance() > ETALON_DISTANCE){
                Drive(ROBOT_SPEED, (rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
            }else{
                DriveSteps = Funnel;
            }
            break;
        
        case Funnel:
            switch (FunnelStep)
            {
            case WallRide:
                errValue = rightDistanceSensor.readDistance() - ETALON_DISTANCE * StepsCount;
                Drive(ROBOT_SPEED, PDreg->update(errValue));
                if (forwardDistanceSensor.readDistance() < ETALON_DISTANCE * StepsCount){
                    FunnelStep = Turn;
                    rightMotor.resetEncoder();
                    leftMotor.resetEncoder();
                    StepsCount++;
                }
                if (StepsCount > BASE_STEP_COUNT){
                    DriveSteps = Base;
                }
                break;
            
            case Turn:
                if (IS_GYRO){
                    if (abs(chopDegrees(chopDegrees(90 * StepsCount) - gyro.getOrientation().x)) > ANGLE_ERROR){
                        Drive(0.0f, -ROBOT_SPEED);
                    }else{
                        FunnelStep = WallRide;
                    }
                }else{
                    if (forwardDistanceSensor.readDistance() < ETALON_DISTANCE * StepsCount){
                        Drive(0.0f, -ROBOT_SPEED);
                    }else{
                        FunnelStep = WallRide;
                    }
                }
                break;
            }
            
            break;

        case Base:
            if (!IS_GYRO){
                DriveSteps = RandomRide;
                break;
            }
            switch (BaseStep)
            {
            case TurnMinusNinety:
                Drive(0.0f, chopDegrees(-90.0 - gyro.getOrientation().x));
                if (abs(chopDegrees(-90.0f - gyro.getOrientation().x)) < ANGLE_ERROR){
                    BaseStep = RightDrive;
                }
                break;
            
            case RightDrive:
                if (forwardDistanceSensor.readDistance() > ETALON_DISTANCE){
                    Drive(ROBOT_SPEED, (rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
                }else{
                    BaseStep = TurnZero;
                }
                break;
            
            case TurnZero:
                Drive(0.0f, - gyro.getOrientation().x);
                if (abs(gyro.getOrientation().x) < ANGLE_ERROR){
                    BaseStep = DownDrive;

                    rightMotor.resetEncoder();
                    leftMotor.resetEncoder();
                }
                break;
            
            case DownDrive:
                if (forwardDistanceSensor.readDistance() > ETALON_DISTANCE){
                    Drive(ROBOT_SPEED, (rightMotor.getCurrentPosition() - leftMotor.getCurrentPosition()));
                }else{
                    DriveSteps = RandomRide;
                }
                break;
            }
            break;

        case RandomRide:
            switch ()
            {
            case GenerateAngle:
                randomAngle = randomFloat(-180.0, 180.0);
                leftMotor.softwareEncoderReset();
                rightMotor.softwareEncoderReset();
                break;
            
            case Turn:
                if (IS_GYRO){
                    Drive(0.0f, randomAngle - gyro.getOrientation().x);
                    if (abs(randomAngle - gyro.getOrientation().x) < ANGLE_ERROR){
                        BaseStep = RightDrive;
                }
                }else{
                    Drive(0.0f, -ROBOT_SPEED);
                    if((leftMotor.getCurrentPosition() + rightMotor.getCurrentPosition()) / 2){

                    }
                }
                break;
            
            case DriveToWall:
                break;
            }
            break;
        }
    }
};

PDRegulator PDreg(0.1f, 0.1f);
DriveTrain driveTrain(PDreg);