#pragma once

#include <Arduino.h>
#include "DriveTrain/DriveSamples.h"
#include "utils/Queue.h"

Queue<DriveSample*> _trajectory;

void driveTrainBegin(){
    PDRegulator _TBLC_PD(0.1f, 0.1f); // надо норм каэфициенты!
    PDRegulator _TBGC_PD(0.1f, 0.1f); // надо норм каэфициенты!
    PDRegulator _TBEV_PD(0.1f, 0.1f); // надо норм каэфициенты!
    PDRegulator _DATW_PD(0.1f, 0.1f); // надо норм каэфициенты!
    PDRegulator _TTTW_PD(0.1f, 0.1f); // надо норм каэфициенты!
    PDRegulator _DFTTL_PD(0.1f, 0.1f); // надо норм каэфициенты!

    _trajectory.enqueue(new DriveForwardToTheLimit(_DFTTL_PD, ETALON_DISTANCE));

}

void driveTrainStart(){
    if(!_trajectory.isEmpty())
        _trajectory.front()->Start();
}

void driveTrainUpdate(){
    if (_trajectory.isEmpty()){
        return;
    }

    if (_trajectory.front()->Execute()){
        _trajectory.dequeue();

        if(!_trajectory.isEmpty())
            _trajectory.front()->Start();
    }
}