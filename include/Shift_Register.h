#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include <Arduino.h>

#define ST_CP 26
#define SH_CP 27
#define DS 25               //data pin
#define OUTPUT_ENABLE 14    //enable output

class ShiftRegister {
    private:
        uint8_t max_output;
    public:
        ShiftRegister(const uint8_t);
        
        void setMaxOutput(uint8_t); //set Register length
        uint8_t getMaxOutput(void); //return Register length
        void write(uint8_t*);        //write to register byte array
};

#endif