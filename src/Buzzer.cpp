#include <Arduino.h>
#include "Buzzer.h"


Buzzer::Buzzer(uint8_t pin) {
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
}


void Buzzer::set(float frequency, float duty) {
    float cycle = 1000.0 / frequency;
    float high = cycle * (duty / 100.0);
    float low = cycle - high;
    this->delays[HIGH] = high;
    this->delays[LOW] = low;
}


void Buzzer::on(float frequency, float duty, unsigned long duration) {
    this->duration = duration ? duration : 0;
    this->set(frequency, duty);
    toggle();
    this->startTime = millis();
}


void Buzzer::off() {
    digitalWrite(this->pin, LOW);
    this->state = OFF;
}


void Buzzer::toggle() {
    this->state = (this->state == HIGH) ? LOW : HIGH;
    digitalWrite(this->pin, this->state);
    this->splitTime = millis();
}


void Buzzer::update() {
    if (this->state == OFF) return;

    unsigned long ms = millis();

    if (this->duration && ms - this->startTime >= this->duration) {
        this->off();
    } else if (ms - this->splitTime >= this->delays[this->state]) {
        this->toggle();
    }
}
