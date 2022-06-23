#include "..\include\Valve.h"

#define ST_CP 26
#define SH_CP 27
#define DS 25
#define OUTPUT_ENABLE 14

uint8_t Valve::output[] = {0,0,0,0,0,0,0,0};
uint8_t Valve::VALVES = 0;

void Valve::writeToRegister(uint8_t value){
    output[this->getNumber()] = value;
    //shift
    digitalWrite(OUTPUT_ENABLE, HIGH);
    for (int8_t i = 7; i >=0; i--){
        digitalWrite(DS, output[i]);
        digitalWrite(ST_CP, LOW);
        digitalWrite(SH_CP, HIGH);
        digitalWrite(SH_CP, LOW);
        digitalWrite(ST_CP, HIGH);
        digitalWrite(ST_CP, LOW);
    }
    digitalWrite(OUTPUT_ENABLE, LOW);

}

bool Valve::isOpen() const {
    return this->isOpened;
}

void Valve::setOpen(bool value){
    this->isOpened = value;
}

bool Valve::getGate() const{
    return this->gate;
}

void Valve::setGate(bool value){
    this->gate = value;
}

void Valve::closeValve(){
    setOpen(false);
    writeToRegister(0);
}

void Valve::openValve(){
    setOpen(true);
    writeToRegister(1);
}

Bound* Valve::getBounds() const{
    return this->bounds;
}

void Valve::setBounds(uint8_t value){
    this->noBounds = value;
    this->bounds = new Bound[value];
}

void Valve::setNumber(uint8_t n){
    this->number = n;
}

uint8_t Valve::getNumber() const{
    return this->number;
}



uint8_t Valve::getNoBounds() const{
    return this->noBounds;
}

void Valve::setNoBounds(uint8_t n){
    this->noBounds = n;
}

Valve::Valve(){
    setGate(false);
    setNoBounds(1);
    setNumber(VALVES);
    closeValve();
    VALVES++;
}

