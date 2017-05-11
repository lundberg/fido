#ifndef __BUZZER__
#define __BUZZER__

#define OFF -1

#include <Arduino.h>

class Buzzer {

public:
    Buzzer(uint8_t pin);

    void set(float frequency, float duty = 50.0);
    void on(float frequency = 1.0, float duty = 50.0, unsigned long duration = 0);
    void off();
    void update();

private:
    uint8_t pin;
    int8_t state = OFF;

    float delays[2];
    unsigned long startTime;
    unsigned long splitTime;
    unsigned long duration;

    void toggle();

};

#endif
