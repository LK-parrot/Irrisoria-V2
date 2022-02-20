#include "Manager/Menu_Manager.h"



String_Manager* Menu_Manager::getStringManager() const{
    return this->sm;
}
Button_Manager* Menu_Manager::getButtonManager() const{
    return this->bm;
}
void Menu_Manager::setStringManager(String_Manager* value){
    this->sm = value;
}
void Menu_Manager::setButtonManager(Button_Manager* value){
    this->bm=value;
}
Menu_Manager::Menu_Manager(){
    Serial.println("Creando mm");
    setStringManager(nullptr);
    Serial.println("Assegnato String Manager");
    setButtonManager(new Button_Manager (19, 18, 17, 16, 15));
    Serial.println("Creato pulsantiera");
}
Menu_Manager::Menu_Manager(uint8_t value){
    setStringManager(new String_Manager(value));
}
Menu_Manager::~Menu_Manager(){
    Serial.println("distrutto");
    delete sm;
    delete bm;
    sm = nullptr;
    bm = nullptr;
}