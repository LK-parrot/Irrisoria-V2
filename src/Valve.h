#ifndef VALVE_H
#define VALVE_H

#include <Arduino.h>
#include "Bound.h"

class Valve{

    private:
        const uint8_t MAX_BOUNDS = 3;
        
        uint8_t number, noBounds;
         
        bool isOpened, gate;
        Bound *bounds;

    public:
    static uint8_t VALVES;
    static const uint8_t pin = 13;
    static const uint8_t MAX_VALVE = 8;
    static uint8_t output[];
    
    
    
    Valve();
    
    bool isOpen() const;
    bool getGate() const;
    Bound* getBounds() const;
    uint8_t getNumber() const;
    uint8_t getNoBounds() const;
    
    void setNoBounds(uint8_t);
    void setBounds(uint8_t);
    void setOpen(bool);
    void setGate(bool);
    void setNumber(uint8_t);
    void closeValve();
    void openValve();
    void writeToRegister(uint8_t);

    bool difference(uint8_t);
};

#endif