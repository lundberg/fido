#include "Dispenser.h"
#include <Arduino.h>

Dispenser::Dispenser(uint8_t pinForward, uint8_t pinBackward, uint8_t pinEnable) {
    this->pinForward = pinForward;
    this->pinBackward = pinBackward;
    this->pinEnable = pinEnable;

    pinMode(pinForward, OUTPUT);
    pinMode(pinBackward, OUTPUT);
    pinMode(pinEnable, OUTPUT);

    digitalWrite(this->pinForward, HIGH);
    digitalWrite(this->pinEnable, LOW);
}

void Dispenser::start() {
    this->setSpeed(50);
    this->running = true;
}

void Dispenser::stop() {
    digitalWrite(this->pinEnable, LOW);
    this->running = false;
}

void Dispenser::setSpeed(uint8_t speed) {
    uint8_t duty = (uint8_t) (255 / 100 * speed);
    analogWrite(this->pinEnable, duty);
}

bool Dispenser::isRunning() {
    return this->running;
}