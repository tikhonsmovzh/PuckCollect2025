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

    float random_float(float min, float max) { // это из инета
        return ((float)rand() / RAND_MAX) * (max - min) + min;
    }

    Steps DriveSteps;
    FunnelSteps FunnelStep;
    BaseSteps BaseStep;
    PDRegulator *PDreg;
    int StepsCount;
    float errValue; // почему то, ни одна моя прога без них не обходится 0_о
public:
    DriveTrain(PDRegulator& PDr){
        DriveSteps = Diagonal;
        FunnelStep = WallRide;
        BaseStep = TurnMinusNinety;
        PDreg = &PDr;
        StepsCount = 1;
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
                if (abs(leftDistanceSensor.readDistance() - rightDistanceSensor.readDistance()) < 70.0f){
                    DriveSteps = Base;
                }
                break;
            
            case Turn:
                if (IS_GYRO){
                    if (abs(abs(chopDegrees(90 * StepsCount)) - abs(gyro.getOrientation().x)) > ANGLE_ERROR){
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
                Drive(0.0f, -90.0 - gyro.getOrientation().x);
                if (abs(-90.0f - gyro.getOrientation().x) < ANGLE_ERROR){
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
                
                break;
            
            case Turn:
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