#ifndef __DISPENSER__
#define __DISPENSER__

#include <inttypes.h>

class Dispenser {

public:
    Dispenser(uint8_t pinForward, uint8_t pinBackward, uint8_t pinEnable);

    void start();
    void stop();

    void setSpeed(uint8_t speed);
    bool isRunning();

private:
    uint8_t pinForward;
    uint8_t pinBackward;
    uint8_t pinEnable;
    bool running = false;

};

#endif