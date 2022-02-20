#include "String_Manager.h"


Valve* String_Manager::getValves() const{
    return this->valves;
}

String_Manager::~String_Manager(){
    delete valves;
    valves = nullptr;
}

void String_Manager::setValves(Valve* value){
    this->valves = value;
}

String_Manager::String_Manager(uint8_t n){
    setValves(new Valve [n]);
}

void String_Manager::initPin(){
    pinMode(Valve::pin, OUTPUT);
}