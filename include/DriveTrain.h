#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"


struct Robot{
    DcMotor* LeftWheel,* RightWheel;
    BNO055Gyro* GyroSensor;
    DistanceSensor* LeftDistSensor,* RightDistSensor,* ForwardDistSensor;

    Robot(DcMotor& LeftMotor,
    DcMotor& RightMotor,
    BNO055Gyro& GyroS,
    DistanceSensor& LeftDistS,
    DistanceSensor& RightDistS,
    DistanceSensor& ForwardDistS){
        LeftWheel = &LeftMotor;
        RightWheel = &RightMotor;
        GyroSensor = &GyroS;
        LeftDistSensor = &LeftDistS;
        RightDistSensor = &RightDistS;
        ForwardDistSensor = &ForwardDistS;

        RightWheel->setDirection(false); // надо будет переделать, когда робот будет (тих, пока что на тебе, тк у меня блютуз модуля нет :) )
    }

    void Drive(float forward, float turn){
        LeftWheel->setPower(forward + turn); // спасибо тежан :)
        RightWheel->setPower(forward - turn);
    }

    float GetOriantation(){
        return GyroSensor->getOrientation().x; // надо будет свапнуть х на то что будет
    }
};

enum Steps{
    Diagonal,
    Funnel,
    Base
};

class Driver{
private:
    Robot* mainRobot;

    Steps DriveSteps;
    float junkValues; // почему то, ни одна моя прога без них не обходится 0_о
public:
    Driver(Robot& robot){
        mainRobot = &robot;
        DriveSteps = Diagonal;
        junkValues = 0.0f;
    }

    void update(){
        switch (DriveSteps){
        case Diagonal:
            if (IS_GYRO){
                if (abs(mainRobot->GetOriantation() - 45) < ANGLE_ERROR){
                    mainRobot->Drive(0.0f, 0.0f);
                    DriveSteps = Funnel;
                    break;
                }else{
                    if (mainRobot->RightWheel->getPower() == 0.0f){
                        mainRobot->Drive(0.0f, -ROBOT_SPEED);
                    }
                    break;
                }
            }else{
                if(junkValues == 0.0f){
                    mainRobot->RightWheel->softwareEncoderReset();
                    mainRobot->LeftWheel->softwareEncoderReset();
                    mainRobot->Drive(0.0f, -ROBOT_SPEED);

                    junkValues = 1.0f;
                }else{
                    if ((mainRobot->RightWheel->getCurrentPosition() + mainRobot->LeftWheel->getCurrentPosition()) * SINGLE_WHEEL_STEP_BY_ENCODER_TIC == TURN_45_DEGREE_DISTANCE){
                        mainRobot->Drive(0.0f, 0.0f);
                        DriveSteps = Funnel;
                    }
                }
            }
            break;

        case Funnel:
            break;

        case Base:
            break;

        default:
            break;
        }
    }
};