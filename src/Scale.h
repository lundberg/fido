#ifndef __SCALE__
#define __SCALE__

#ifndef SCALE_DT_PIN
#define SCALE_DT_PIN 11
#endif
#ifndef SCALE_SCK_PIN
#define SCALE_SCK_PIN 12
#endif

#include <HX711.h>

HX711 _scale(SCALE_DT_PIN, SCALE_SCK_PIN);

struct ScaleClass {

    long weight = 0;

    void calibrate(float value) {
        _scale.set_scale(value * -1);
        _scale.tare(10);
//        _scale.set_scale();
//        _scale.tare();
//        Serial.print("1. Place known weight on scale and enter its value: ");
//
//        while (!Serial.available()) {}
//        String data = Serial.readStringUntil('\n');
//        while (Serial.available()) {
//            Serial.read();
//        }
//
//        float knownWeight = data.toFloat();
//        float units = _scale.get_units(10);
//        float scale = units / knownWeight;
//
//        Serial.print("\r\nKnown weight: "); Serial.println(knownWeight);
//        Serial.print("Raw value: "); Serial.println(units);
//        Serial.print("Scale: "); Serial.println(scale);
//
//        Serial.print("2. Remove weight and press <enter>");
//        while (!Serial.available()) {}
//        Serial.readStringUntil('\n');
//        while (Serial.available()) {
//            Serial.read();
//        }
//
//        _scale.set_scale(scale);
//        _scale.tare(10);
    }

    bool read() {
        long value = _scale.read(false);

        #ifdef DEBUG
        Serial.print("scale read: "); Serial.println(value);
        #endif

        if (value != UNDEFINED) {
            value -= _scale.get_offset();
            weight = (long) ((value / _scale.get_scale()) + 0.5);
            if (weight < 0) weight = 0;

            #ifdef DEBUG
            Serial.print("weight: "); Serial.println(weight);
            #endif

            return true;
        }

        return false;
    }

};

static ScaleClass Scale;

#endif