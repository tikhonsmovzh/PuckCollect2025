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
    chaoticDriving
};



tripAlgorithm myTrip = diagonalPassage;
diagonalPassageTask diagonalPassageObj;



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

class diagonalPassageTask : DriveSample{
public:
    bool Execute() override{
        if (driveToWall(ETALON_DISTANCE)){
                return false;
        }
        return true;
    }
};