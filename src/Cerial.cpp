#include <Arduino.h>

#include "Cerial.h"


Cerial::Cerial(Handler handler) {
    this->handler = handler;
}


void Cerial::begin() {
    Serial.begin(9600);
    while (!Serial) {;}
    Serial.println("--- Press m for menu ---");
    Serial.flush();
}


void Cerial::update() {
    if (!Serial.available())
        return;

    char c = (char) Serial.read();

    if (state == IDLE && c == 'm') {
        Serial.print(F(
            "=== Control Panel ===\r\n"
            "1. Set time\r\n"
            "2. Set date\r\n"
            "3. Set alarm 1\r\n"
            "4. Set alarm 2\r\n"
            "5. Set target weight\r\n"
            "6. Set scale calibration\r\n"
            "? "
        ));
        state = MENU;

        handler(state, 0);

    } else if (state == MENU && 49 <= c && c <= 54) {
        Serial.println(c);  // Echo
        Serial.print("> ");

        switch (c) {
            case '1': state = SET | TIME; break;
            case '2': state = SET | DATE; break;
            case '3': state = SET | ALARM_ONE; break;
            case '4': state = SET | ALARM_TWO; break;
            case '5': state = SET | WEIGHT; break;
            case '6': state = SET | SCALE; break;
        }

    } else if (state & SET) {
        Serial.print(c);  // Echo
        buffer[buffer_length++] = c;

        if (c == '\n') {
            // Call back to handler
            uint16_t value = atoi(buffer);
            handler(state & ~SET, value);

            // Clear buffer
            while (buffer_length > 0) {
                buffer_length--;
                buffer[buffer_length] = 0x00;
            }
            state = IDLE;
        }
    }
}
