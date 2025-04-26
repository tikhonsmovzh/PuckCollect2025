#pragma once

#include <Arduino.h>
#include "DriveTrain/DriveTrainV2.h"
#include "utils/Queue.h"

void BodyControl(Queue<SimpleAction> algorithm){
    if (algorithm.isEmpty()){
        return;
    }


    switch(algorithm.front().name)
    {
    case DriveToWall:
        if (DriveForwardToTheLimitObj.Execute(algorithm.front().arg)){
            algorithm.dequeue();
        }
        break;
    
    case TurnOnWall:
        if (TurnToTheWallObj.Execute(algorithm.front().arg)){
            algorithm.dequeue();
        }
        break;
    
    case DriveAlongWall:
        if (DrivingAlongTheWall.Execute(algorithm.front().arg)){
            algorithm.dequeue();
        }
        break;
    }
}