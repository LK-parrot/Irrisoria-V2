#include "Button_Manager.h"

Button_Manager::Button_Manager(uint8_t left, uint8_t right, uint8_t menu, uint8_t up, uint8_t down){
    this->left=new Button(left);
    this->right=new Button(right);
    this->menu=new Button(menu);
    this->up=new Button(up);
    this->down=new Button(down);
}

Button_Manager::~Button_Manager(){
    delete left;
    delete right;
    delete menu;
    delete up;
    delete down;
    left = nullptr;
    right = nullptr;
    menu = nullptr;
    up = nullptr;
    down = nullptr;
}

Button* Button_Manager::getLeft() const{
    return this->left;
}
Button* Button_Manager::getRight() const{
    return this->right;
}
Button* Button_Manager::getMenu() const{
    return this->menu;
}
Button* Button_Manager::getUp() const{
    return this->up;
}
Button* Button_Manager::getDown() const{
    return this->down;
}