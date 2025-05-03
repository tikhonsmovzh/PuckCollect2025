#pragma once

#include <Arduino.h>
#include "DriveTrain/DriveTrainV2.h"
#include "utils/Queue.h"

void beginQueue(){
    PDRegulator _TBLC_PD(0.1f, 0.1f); // надо норм каэфициенты!
    PDRegulator _TBGC_PD(0.1f, 0.1f); // надо норм каэфициенты!
    PDRegulator _TBEV_PD(0.1f, 0.1f); // надо норм каэфициенты!
    PDRegulator _DATW_PD(0.1f, 0.1f); // надо норм каэфициенты!
    PDRegulator _TTTW_PD(0.1f, 0.1f); // надо норм каэфициенты!
    PDRegulator _DFTTL_PD(0.1f, 0.1f); // надо норм каэфициенты!

    myQueue.enqueue(new DriveForwardToTheLimit(_DFTTL_PD, ETALON_DISTANCE));

}

void BodyControl(Queue<DriveSample*> algorithm){
    if (algorithm.isEmpty()){
        return;
    }

    if (algorithm.front()->Execute()){
        algorithm.dequeue();
        algorithm.front()->Start();
    }
}