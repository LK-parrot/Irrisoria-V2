#ifndef STRING_MANAGER_H
#define STRING_MANAGER_H
#include "Valve.h"

class String_Manager{
private:
    Valve* valves = nullptr;
    

public:
    String_Manager(uint8_t);
    ~String_Manager();
    Valve* getValves() const;
    void setValves(Valve*);
    static void initPin();
};

#endif