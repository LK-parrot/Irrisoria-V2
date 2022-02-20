#include <Bound.h>


uint8_t Bound::getFromH() const{
    return this->fromH;
}
uint8_t Bound::getToH() const{
    return this->toH;
}
uint8_t Bound::getFromM() const{
    return this->fromM;
}
uint8_t Bound::getToM() const{
    return this->toM;
}

void Bound::setFromH(uint8_t h){
    this->fromH = h;
}
void Bound::setToH(uint8_t h){
    this->toH = h;
}
void Bound::setFromM(uint8_t m){
    this->fromM = m;
}
void Bound::setToM(uint8_t m){
    this->toM = m;
}

String Bound::getTime() const{
    return String(getFromH()) + ":" + ((getFromM() > 9) ? String(getFromM()) : ("0" + String(getFromM()))) + " - " + String(getToH()) + ":" + ((getToM() > 9) ? String(getToM()) : ("0" + String(getToM())));
}

Bound::Bound(){
    setFromH(0);
    setToH(0);
    setFromM(0);
    setToM(0);
}