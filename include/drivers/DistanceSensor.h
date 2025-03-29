#pragma once

#include <Arduino.h>
#include "../utils/ElapsedTime.h"

#define SOUND_SPEED (0.03313f + 0.0000606f * 20.0f)
#define ENABLE_TRIGER_TIMER 5
#define MAX_DISTANCE 400
#define DISTANCE_SENSOR_TIME_OUT (MAX_DISTANCE / SOUND_SPEED) + 2

#define DISTANCE_SENSOR_UPDATE_TIME 6

class DistanceSensor{
private:
    uint8_t _triggerPin, _echoPin;
    uint32_t _lastUpdateTimer = 0;
    
    uint16_t _lastDistance = 0;

public:
    DistanceSensor(uint8_t triggerPin, uint8_t echoPin){
        _triggerPin = triggerPin;
        _echoPin = echoPin;
    }

    void begin(){
        pinMode(_triggerPin, OUTPUT);
        pinMode(_echoPin, INPUT);
    }

    uint16_t getDistance(){
        if(millis() - _lastUpdateTimer > DISTANCE_SENSOR_UPDATE_TIME){
            digitalWrite(_triggerPin, 1);
            delayMicroseconds(ENABLE_TRIGER_TIMER);
            digitalWrite(_triggerPin, 0);

            uint64_t time = pulseIn(_echoPin, 1, DISTANCE_SENSOR_TIME_OUT);

            uint16_t distance = time * SOUND_SPEED;

            if(distance > MAX_DISTANCE)
                distance = MAX_DISTANCE;

            _lastUpdateTimer = millis();

            _lastDistance = distance;
        }

        return _lastDistance;
    }
};