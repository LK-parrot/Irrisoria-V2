
#ifndef BOUND_H
#define BOUND_H

#include <Arduino.h>
#include <string>


class Bound{
    private:
        uint8_t fromH, fromM, toH, toM;
    public:

    uint8_t getFromH() const;
    uint8_t getToH() const;
    uint8_t getFromM() const;
    uint8_t getToM() const;

    void setFromH(uint8_t);
    void setToH(uint8_t);
    void setFromM(uint8_t);
    void setToM(uint8_t);
    String getTime() const;
    Bound();
};

#endif