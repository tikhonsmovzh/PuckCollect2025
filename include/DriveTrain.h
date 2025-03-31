#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"

struct Robot{
    DcMotor *LeftWheel, *RightWheel;
    BNO055Gyro *GyroSensor;
    DistanceSensor *LeftDistSensor, *RightDistSensor, *ForwardDistSensor;

    init(DcMotor *LeftMotor,
    DcMotor *RightMotor,
    BNO055Gyro *GyroS,
    DistanceSensor *LeftDistS,
    DistanceSensor *RightDistS,
    DistanceSensor *ForwardDistS){
        LeftWheel = LeftMotor;
        RightWheel = RightMotor;
        GyroSensor = GyroS;
        LeftDistSensor = LeftDistS;
        RightDistSensor = RightDistS;
        ForwardDistSensor = ForwardDistS;

        RightWheel->setDirection(false); // надо будет переделать, когда робот будет (тих, пока что на тебе, тк у меня блютуз модуля нет :) )
    }

    MotorForward(float speed){
        LeftWheel->setPower(speed);
        RightWheel->setPower(speed);
    }

    MotorBackward(float speed){
        LeftWheel->setPower(-speed);
        RightWheel->setPower(-speed);
    }

    MotorTurnRight(float speed){
        LeftWheel->setPower(speed);
        RightWheel->setPower(-speed);
    }

    MotorTurnLeft(float speed){
        LeftWheel->setPower(-speed);
        RightWheel->setPower(speed);
    }

    FreeMotorControl(float speedRight, float speedLeft){
        LeftWheel->setPower(speedLeft);
        RightWheel->setPower(speedRight);
    }

    RideDistanceForward(float steps, float speed){
        RightWheel->resetEncoder();
        LeftWheel->resetEncoder();
        MotorForward(speed);
        while (RightWheel->getCurrentPosition() + LeftWheel->getCurrentPosition() < steps * SINGLE_ENCODER_STEP * 2);
    }
};

void DiagonalTravel(Robot MyLimbs){
    MyLimbs.MotorForward(0.5f);
    if (IS_DISTANCE_SENSOR){
        while(MyLimbs.ForwardDistSensor->readDistance() > 10);
    }else{
        MyLimbs.RideDistanceForward(5, 0.5);  // пока что так, потом помереем *5 - это полных оборотов колес
    }
}