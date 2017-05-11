#ifndef MAIN_H
#define MAIN_H

#include "Cerial.h"

#define DISPLAY_DIO_PIN 4
#define DISPLAY_CLK_PIN 5

#define BUZZER_PIN 3

#define DISPENSER_FORWARD_PIN 7
#define DISPENSER_BACKWARD_PIN 8
#define DISPENSER_ENABLE_PIN 9

enum class State {
    IDLE  = 1 << 0,
    ALARM = 1 << 1,
    FEED  = 1 << 2
};

State state = State::IDLE;
//uint16_t currentWeight = -1;
long past;

void commandHandler(ManagerState trigger, uint16_t value);

#endif
