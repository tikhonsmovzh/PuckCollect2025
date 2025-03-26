#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SoftWire.h>

class IWire
{
public:
    virtual void begin();
    virtual void setClock(uint32_t);
    virtual void beginTransmission(uint8_t) = 0;
    virtual void endTransmission() = 0;
    virtual void write(uint8_t) = 0;
    virtual uint8_t read() = 0;
    virtual void requestFrom(uint8_t, uint8_t) = 0;
    virtual void readBytes(uint8_t *, size_t) = 0;
    virtual uint16_t available() = 0;

    void write8(uint8_t addr, uint8_t data)
    {
        beginTransmission(addr);
        write(data);
        endTransmission();
    }

    void write2x8(uint8_t addr, uint8_t data)
    {
        beginTransmission(addr);
        write(data);
        endTransmission();
    }
};

class HardwareWire : public IWire
{
public:
    void begin()
    {
        Wire.begin();
    }

    void setClock(uint32_t clock)
    {
        Wire.setClock(clock);
    }

    void beginTransmission(uint8_t addr)
    {
        Wire.beginTransmission(addr);
    }

    void endTransmission()
    {
        Wire.endTransmission();
    }

    void write(uint8_t data)
    {
        Wire.write(data);
    }

    uint8_t read()
    {
        return Wire.read();
    }

    void requestFrom(uint8_t addr, uint8_t count)
    {
        Wire.requestFrom(addr, count);
    }

    void readBytes(uint8_t *buffer, size_t length)
    {
        Wire.readBytes(buffer, length);
    }

    uint16_t available()
    {
        return Wire.available();
    }
};

class SoftwareWire : public IWire
{
    SoftWire *_softWire;

public:
    SoftwareWire(uint8_t sda, uint8_t scl)
    {
        _softWire = new SoftWire(sda, scl);
    }

    void begin()
    {
        _softWire->begin();
    }

    void setClock(uint32_t clock)
    {
        _softWire->setClock(clock);
    }

    void beginTransmission(uint8_t addr)
    {
        _softWire->beginTransmission(addr);
    }

    void endTransmission()
    {
        _softWire->endTransmission();
    }

    void write(uint8_t data)
    {
        _softWire->write(data);
    }

    uint8_t read()
    {
        return _softWire->read();
    }

    void requestFrom(uint8_t addr, uint8_t count)
    {
        _softWire->requestFrom(addr, count);
    }

    void readBytes(uint8_t *buffer, size_t length)
    {
        _softWire->readBytes(buffer, length);
    }

    uint16_t available()
    {
        return _softWire->available();
    }
};