#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <Arduino.h>
#include "Manager/Button_Manager.h"
#include "Manager/String_Manager.h"


class Menu_Manager{
private:
    String_Manager* sm;
    Button_Manager* bm;
public:
    String_Manager* getStringManager() const;
    Button_Manager* getButtonManager() const;
    void setButtonManager(Button_Manager*);
    void setStringManager(String_Manager*);

    void askAmountValves();

    Menu_Manager();
    Menu_Manager(uint8_t);
    ~Menu_Manager();
};


#endif