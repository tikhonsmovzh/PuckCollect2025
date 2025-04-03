#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"

<<<<<<< Updated upstream

struct Robot{
    DcMotor* LeftWheel,* RightWheel;
    BNO055Gyro* GyroSensor;
    DistanceSensor* LeftDistSensor,* RightDistSensor,* ForwardDistSensor;

=======
struct Robot{
    DcMotor* LeftWheel,* RightWheel;
    BNO055Gyro* GyroSensor;
    DistanceSensor* LeftDistSensor,* RightDistSensor,* ForwardDistSensor;

>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
    }

    float GetOriantation(){
        return GyroSensor->getOrientation().x; // надо будет свапнуть х на то что будет
=======
>>>>>>> Stashed changes
    }
};

enum Steps{
    Diagonal,
    Funnel,
    Base
<<<<<<< Updated upstream
};
=======
}
>>>>>>> Stashed changes

class Driver{
private:
    Robot* mainRobot;

<<<<<<< Updated upstream
    Steps DriveSteps;
=======
    Steps driveSteps;
>>>>>>> Stashed changes
    float junkValues; // почему то, ни одна моя прога без них не обходится 0_о
public:
    Driver(Robot& robot){
        mainRobot = &robot;
<<<<<<< Updated upstream
        DriveSteps = Diagonal;
=======
        driveSteps = Diagonal;
>>>>>>> Stashed changes
        junkValues = 0.0f;
    }

    void update(){
        switch (DriveSteps){
        case Diagonal:
            if (IS_GYRO){
<<<<<<< Updated upstream
                if (abs(mainRobot->GetOriantation() - 45) < ANGLE_ERROR){
                    mainRobot->Drive(0.0f, 0.0f);
                    DriveSteps = Funnel;
                    break;
                }else{
                    if (mainRobot->RightWheel->getPower() == 0.0f){
                        mainRobot->Drive(0.0f, -ROBOT_SPEED);
=======
                switch (mainRobot.GyroS)
                {
                case 45:
                    mainRobot.Drive(0.0f, 0.0f);
                    DriveSteps = Funnel;
                    break;
                default:
                    if (mainRobot.getPower == 0.0f){
                        mainRobot.Drive(0.0f, -ROBOT_SPEED);
>>>>>>> Stashed changes
                    }
                    break;
                }
            }else{
<<<<<<< Updated upstream
                if(junkValues == 0.0f){
                    mainRobot->RightWheel->softwareEncoderReset();
                    mainRobot->LeftWheel->softwareEncoderReset();
                    mainRobot->Drive(0.0f, -ROBOT_SPEED);

                    junkValues = 1.0f;
                }else{
                    if ((mainRobot->RightWheel->getCurrentPosition() + mainRobot->LeftWheel->getCurrentPosition()) * SINGLE_WHEEL_STEP_BY_ENCODER_TIC == TURN_45_DEGREE_DISTANCE){
                        mainRobot->Drive(0.0f, 0.0f);
=======
                if(junkValue == 0.0f){
                    mainRobot.RightWheel.softwareEncoderReset();
                    mainRobot.LeftWheel.softwareEncoderReset();
                    mainRobot.Drive(0.0f, -ROBOT_SPEED);

                    junkValue = 1f;
                }else{
                    if ((mainRobot.RightWheel.getCurrentPosition() + mainRobot.LeftWheel.getCurrentPosition()) * SINGLE_WHEEL_STEP_BY_ENCODER_TIC == TURN_45_DEGREE_DISTANCE){
                        mainRobot.Drive(0.0f, 0.0f);
>>>>>>> Stashed changes
                        DriveSteps = Funnel;
                    }
                }
            }
            break;

        case Funnel:
            break;

<<<<<<< Updated upstream
        case Base:
=======
        case Base && IS_GYRO:
>>>>>>> Stashed changes
            break;

        default:
            break;
        }
    }
};