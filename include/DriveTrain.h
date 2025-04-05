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
    PDRegulator *PDreg;
    int StepsCount;
    float errValue; // почему то, ни одна моя прога без них не обходится 0_о
public:
    DriveTrain(PDRegulator& PDr){
        DriveSteps = Diagonal;
        FunnelStep = WallRide;
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
            break;

        case RandomRide:
            break;
        }
    }
};

PDRegulator PDreg(0.1f, 0.1f);
DriveTrain driveTrain(PDreg);