#include "../include/Shift_Register.h"

void ShiftRegister::setMaxOutput(uint8_t max){
    this->max_output = max;
}

uint8_t ShiftRegister::getMaxOutput(void){
    return this->max_output;
}

ShiftRegister::ShiftRegister(uint8_t length){
    pinMode(OUTPUT_ENABLE, OUTPUT);
    digitalWrite(OUTPUT_ENABLE, HIGH);
    pinMode(ST_CP, OUTPUT);
    pinMode(SH_CP, OUTPUT);
    pinMode(DS, OUTPUT);
    
    
    digitalWrite(SH_CP, LOW);
    digitalWrite(ST_CP, LOW);
    digitalWrite(DS, LOW);

    setMaxOutput(length);
}


void ShiftRegister::write(uint8_t *array){
    digitalWrite(OUTPUT_ENABLE, HIGH);
    for (int8_t i = getMaxOutput(); i >=0; i--){
        digitalWrite(DS, *array);
        digitalWrite(ST_CP, LOW);
        digitalWrite(SH_CP, HIGH);
        digitalWrite(SH_CP, LOW);
        digitalWrite(ST_CP, HIGH);
        digitalWrite(ST_CP, LOW);
        array++;
    }
    digitalWrite(OUTPUT_ENABLE, LOW);
}