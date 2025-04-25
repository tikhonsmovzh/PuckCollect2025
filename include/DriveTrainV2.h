#pragma once

#include <Arduino.h>
#include "Devices.h"
#include "Configs.h"
#include "utils/PDRegulator.h"
#include "utils/Sgn.h"
#include "utils/DriveParent.h"
#include "Intake.h"

enum tripAlgorithm{
    diagonalPassage,
    spiralRide,
    returningHome,
    chaoticDriving,
    rollBack
};


class diagonalPassageTask : DriveSample{
public:
    diagonalPassageTask(PDRegulator &PDr, ElapseTime &actionTime){
        PDreg = &PDr;
        ActionTime = &actionTime;
    }


    bool Execute() override{
        if (driveToWall(ETALON_DISTANCE)){
                return false;
        }
        return true;
    }
};



tripAlgorithm myTrip = diagonalPassage;

PDRegulator PDreg(0.1f, 0.1f);
ElapseTime actionTime;
diagonalPassageTask diagonalPassageObj(PDreg, actionTime);


void Update(){
    switch (myTrip)
    {
    case diagonalPassage:
        if (diagonalPassageObj.Execute())
            myTrip = spiralRide;

        break;
    
    case spiralRide:


        break;
    
    case returningHome:


        break;
    
    case chaoticDriving:

        
        break;
    }
}