#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include "Button.h"
#include <Arduino.h>

class Button_Manager{

    private:
        Button* left;
        Button* right;
        Button* menu;
        Button* up;
        Button* down;

    public:
        Button_Manager(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
        ~Button_Manager();
        Button* getLeft() const;
        Button* getRight() const;
        Button* getMenu() const;
        Button* getUp() const;
        Button* getDown() const;
};      

#endif