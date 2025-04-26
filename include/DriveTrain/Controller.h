#pragma once

#include <Arduino.h>
#include "DriveTrain/DriveTrainV2.h"
#include "utils/Queue.h"

void beginQueue(){
    DriveForwardToTheLimitObj.arg = ETALON_DISTANCE;
    myQueue.enqueue(DriveForwardToTheLimitObj);

}

void BodyControl(Queue<DriveSample> algorithm){
    if (algorithm.isEmpty()){
        return;
    }

    if (algorithm.front().Execute()){
        algorithm.dequeue();
        algorithm.front().Start();
    }
}