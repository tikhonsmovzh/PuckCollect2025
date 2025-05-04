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
    if (IS_GYRO) _trajectory.enqueue(new TurnByGlobalCoordinates(_TBGC_PD, 90));
    for (int step = 1; step < 4; step++){ //три круга
        for (int actions = 1; actions < 5; actions++){ //в каждом круге полный проезд, из четырех проездов по стене, и четырех поворотов
            _trajectory.enqueue(new DrivingAlongTheWall(_DATW_PD, ETALON_DISTANCE * step));
            if (IS_GYRO)
                _trajectory.enqueue(new TurnByGlobalCoordinates(_TBGC_PD, chopDegrees(90 + 90 * actions)));
            else
                _trajectory.enqueue(new TurnToTheWall(_TTTW_PD, ETALON_DISTANCE * step));
        }
    }
    if (IS_GYRO){
        _trajectory.enqueue(new TurnByGlobalCoordinates(_TBGC_PD, -90));
        _trajectory.enqueue(new DriveForwardToTheLimit(_DFTTL_PD, ETALON_DISTANCE));
        _trajectory.enqueue(new TurnByGlobalCoordinates(_TBGC_PD, -180));
        _trajectory.enqueue(new DriveForwardToTheLimit(_DFTTL_PD, ETALON_DISTANCE));
        _trajectory.enqueue(new TurnByGlobalCoordinates(_TBGC_PD, 0));
    }
    //остается только дописать генерацию рандомной езды (опционально), или есть идея интереснее, но ее позже расскажу
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