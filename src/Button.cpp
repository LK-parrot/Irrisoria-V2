#include "Button.h"

void Button::setPin(uint8_t pin){
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);
}
uint8_t Button::getPin() const{
    return this->pin;
}
bool Button::isPressed() const{
    return !digitalRead(getPin());   //1 true 0 false (pulldown pullup)
}

Button::Button(uint8_t pin){
    setPin(pin);
}