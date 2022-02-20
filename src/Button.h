#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button{
    private:
        uint8_t pin;
    public:
        void setPin(uint8_t);
        uint8_t getPin() const;
        bool isPressed() const;
        Button(uint8_t);

};

#endif