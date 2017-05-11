#ifndef __SETTINGS__
#define __SETTINGS__

#include <Arduino.h>
#include <EEPROM.h>

struct SettingsClass {

    struct Data {
        uint16_t alarms[2];
        uint16_t scaleCalibration;
        uint16_t scaleTargetWeight;
    } data;

    void begin() {
        Serial.println(F("=== Settings ==="));

        EEPROM.get(0, data);

        if (data.scaleCalibration == UINT16_MAX) {
            Serial.println(F("Creating defaults..."));

            data.alarms[0] = 700;
            data.alarms[1] = 1900;
            data.scaleCalibration = 2050;
            data.scaleTargetWeight = 300;

            save();
        }

        Serial.print("ALARM 1: "); Serial.println(data.alarms[0]);
        Serial.print("ALARM 2: "); Serial.println(data.alarms[1]);
        Serial.print("SCALE CALIBRATION: "); Serial.println(data.scaleCalibration);
        Serial.print("SCALE TARGET WEIGHT: "); Serial.println(data.scaleTargetWeight);
    }

    void save() {
        EEPROM.put(0, data);
    }

};

static SettingsClass Settings;

#endif
