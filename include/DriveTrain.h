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
    float speedK = 1.0f;
    PDRegulator *PDreg;
    int junkValuesI;
    float junkValuesF, errValue; // почему то, ни одна моя прога без них не обходится 0_о
public:
    DriveTrain(PDRegulator& PDr){
        DriveSteps = Diagonal;
        junkValuesF = 0.0f;
        junkValuesI = 2;
        PDreg = &PDr;
    }

    void begin(){

    }

    void start(){

    }

    void update(){
        switch (DriveSteps){
        case Diagonal:
            if (forwardDistanceSensor.readDistance() > SIDE_DISTANCE){
                if (junkValuesI == 2){
                    Drive(ROBOT_SPEED, 0.0f);
                    junkValuesI = 1;
                }
            }else{
                Drive(0.0f, 0.0f);
                DriveSteps = Funnel;
            }
            break;
        
        case Funnel:
            errValue = rightDistanceSensor.readDistance() - ETALON_SIDE_DISTANCE * junkValuesF;
            Drive(ROBOT_SPEED, (PDreg->update(errValue) * speedK) - junkValuesI);

            junkValuesF += 0.5f;

            if (forwardDistanceSensor.readDistance() < SIDE_DISTANCE){
                junkValuesI = 1;
            }else{
                junkValuesI = 0;
            }
            if (abs(rightDistanceSensor.readDistance() - leftDistanceSensor.readDistance()) < 70.0f){
                DriveSteps = Base;
                junkValuesI = 0;
            }
            break;

        case Base:
            if (IS_GYRO){ // здесь junkValueI это своеобразный рычаг (здесь он точно нужен)
                if (junkValuesI == 0){
                    if (abs(240 - gyro.getOrientation().x) > ANGLE_ERROR){
                        Drive(0.0f, ROBOT_SPEED);
                    }else{
                        junkValuesI = 1;
                    }
                }
                if (junkValuesI == 1){
                    if (forwardDistanceSensor.readDistance() > SIDE_DISTANCE){
                        Drive(ROBOT_SPEED, 0.0f);
                    }else{
                        junkValuesI = 2;
                    }
                }
                if(junkValuesI == 2){
                    if (abs(180 - gyro.getOrientation().x) > ANGLE_ERROR){
                        Drive(0.0f, ROBOT_SPEED);
                    }else{
                        junkValuesI = 3;
                    }
                }
                if (junkValuesI == 3){
                    if (forwardDistanceSensor.readDistance() > SIDE_DISTANCE){
                        Drive(ROBOT_SPEED, 0.0f);
                    }else{
                        DriveSteps = RandomRide;
                        junkValuesI = 1;
                    }
                }
            }else{
                DriveSteps = RandomRide;
                junkValuesI = 1;
            }
            break;

        case RandomRide:
            if (junkValuesI == 1){
                junkValuesI = 0;
                rightMotor.softwareEncoderReset();
                leftMotor.softwareEncoderReset();
                junkValuesF = random_float(0.1f, 20.0f);
            }
            if (junkValuesI == 0){
                if (abs(SINGLE_ENCODER_STEP - (rightMotor.getCurrentPosition() + leftMotor.getCurrentPosition()) / 2) > ANGLE_ERROR){
                    Drive(0.0f, -ROBOT_SPEED);
                }else{
                    junkValuesI = 2;
                }
            }
            if (junkValuesI == 2){
                if (forwardDistanceSensor.readDistance() > SIDE_DISTANCE){
                    Drive(ROBOT_SPEED, 0.0f;);
                }
                else{
                    junkValuesI = 1;
                }
            }
            break;
        }
    }
};

PDRegulator PDreg(1.0f, 1.0f);
DriveTrain driveTrain(PDreg);