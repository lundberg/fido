//#define DEBUG
/*
    TODO:
    - Implement alarm 2
    - Serial set date
    - Serial set time
    - Handle negative overflowing to positive weight
    - Better scale calibration
    - Button for trigger alarm/dispenser
    - Implement debug class
*/

#include <math.h>
#include <Arduino.h>
#include <TM1637.h>

#include "main.h"
#include "Cerial.h"
#include "Settings.h"
#include "Clock.h"
#include "Scale.h"
#include "Dispenser.h"
#include "Buzzer.h"

Cerial cerial(commandHandler);
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);
Dispenser dispenser(DISPENSER_FORWARD_PIN, DISPENSER_BACKWARD_PIN, DISPENSER_ENABLE_PIN);
Buzzer buzzer(BUZZER_PIN);


void setup() {
    display.setBrightness(0x0a);
//    display.setSegments(SEG_BOOT);  // TODO: Implement "text" support in display api

    cerial.begin();
    Serial.println("Booting fido..."); Serial.flush();

    Settings.begin();
    Clock.begin();
    Clock.setAlarm(Settings.data.alarms[0]);
    Scale.calibrate(Settings.data.scaleCalibration);
//    buzzer.on(30, 50, 2000);

    Clock.triggerAlarm();
}


void loop() {
    switch (state) {
        case State::IDLE: {
            cerial.update();

            uint8_t alarm = Clock.checkAlarm();

            if (alarm > 0) {
                state = State::ALARM;
            }

            // Display time
            long now = Clock.getTime();
            if (now != past) {
                display.showNumberDec(floor(now / 100), true, 4, 0, now % 2);
                past = now;
            }
        } break;

        case State::ALARM:
            buzzer.on(1, 50);
            dispenser.start();
            state = State::FEED;

        case State::FEED:
            buzzer.update();

            // Display weight
            if (Scale.read()) {
                display.setColon(false);
                display.showNumberDec(Scale.weight);

                #ifdef DEBUG
                Serial.print('Scale: '); Serial.println(Scale.weight);
                #endif

                // Match buzzer frequency to target weight percentage, range 1 - 31 hertz
                float frequency = (float) Scale.weight / Settings.data.scaleTargetWeight;
                frequency *= frequency;
                frequency *= 30.0;
                frequency += 1.0;
                buzzer.set(frequency);
            }

            // Validate target weight
            if (Scale.weight >= Settings.data.scaleTargetWeight) {
                buzzer.off();
                dispenser.stop();
                state = State::IDLE;
            }

            break;
    }
}

void commandHandler(ManagerState trigger, uint16_t value) {
    switch (trigger) {
        case ManagerState::MENU:
//            display.setSegments(SEG_CONF);
            break;

        case ManagerState::TIME:
            Serial.println("TODO: <set time>");
            break;

        case ManagerState::DATE:
            Serial.println("TODO: <set date>");
            break;

        case ManagerState::ALARM_ONE:
            Settings.data.alarms[0] = value;
            Settings.save();
            Clock.setAlarm(value);
            break;

        case ManagerState::WEIGHT:
            Settings.data.scaleTargetWeight = value;
            Settings.save();
            Serial.print("Scale target weight set to: "); Serial.println(value);
            break;

        case ManagerState::SCALE:
            Settings.data.scaleCalibration = value;
            Settings.save();
            Scale.calibrate(value);
            Serial.print("Scale calibrated to: "); Serial.println(value);
            break;

        default: break;
    }
}
