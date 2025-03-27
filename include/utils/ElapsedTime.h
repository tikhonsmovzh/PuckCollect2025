#pragma once

#include <Arduino.h>

class ElapseTime{
private:
    uint64_t _resetMicros = 0;
public:
    ElapseTime(){
        _resetMicros = micros();
    }

    void reset(){
        _resetMicros = micros();
    }

    uint64_t micros(){
        return micros() - _resetMicros;
    }

    float milliseconds(){
        return micros() / 1000.0f;
    }

    float seconds(){
        return micros() / 1000000.0f;
    }
};