#ifndef __CLOCK__
#define __CLOCK__

#ifndef ALARM_PIN
#define ALARM_PIN 2
#endif

#include <Arduino.h>
#include <Wire.h>
#include <RtcDS3231.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))

RtcDS3231 RTC;

volatile bool alarmTriggered = false;
void alarmInterrupt() {
    alarmTriggered = true;
}

void printDateTime(const RtcDateTime& dt) {
	char datestring[20];

	snprintf_P(
	    datestring,
        countof(datestring),
        PSTR("%04u-%02u-%02u %02u:%02u:%02u"),
        dt.Year(),
        dt.Month(),
        dt.Day(),
        dt.Hour(),
        dt.Minute(),
        dt.Second()
    );

	Serial.println(datestring);
}

struct ClockClass {

    void begin() {
        RTC.Begin();

        RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
        RtcDateTime now;

        if (!RTC.GetIsRunning()) {
            Serial.println(F("RTC was not actively running, starting now."));
            RTC.SetIsRunning(true);
        }

        if (!RTC.IsDateTimeValid()) {
            Serial.println(F("RTC lost confidence in the DateTime! Updating with compile time."));
            RTC.SetDateTime(compiled);
            now = compiled;
        } else {
            now = RTC.GetDateTime();

            if (now < compiled) {
                Serial.println(F("RTC is older than compile time! Updating with compile time."));
                RTC.SetDateTime(compiled);
                now = compiled;
            }
        }

        Serial.print("COMPILED: ");
        Serial.print(__DATE__); Serial.print(" @ "); Serial.println(__TIME__);

        Serial.print("TIME: ");
        printDateTime(now);
        Serial.flush();

        // Set up alarm configuration
        RTC.Enable32kHzPin(false);
        RTC.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);

        pinMode(ALARM_PIN, INPUT);
        attachInterrupt(digitalPinToInterrupt(ALARM_PIN), alarmInterrupt, FALLING);
    }

//    void setDate()

    void setAlarm(uint16_t alarm) {
        // Partition hour and minutes, i.e. 745 -> 7 and 45
        uint8_t hour = (uint8_t) floor(alarm / 100.0);
        uint8_t minute = (uint8_t) alarm - (hour * 100);

        DS3231AlarmOne alarm1(0, hour, minute, 0, DS3231AlarmOneControl_HoursMinutesSecondsMatch);

        RTC.SetAlarmOne(alarm1);
        RTC.LatchAlarmsTriggeredFlags();

        Serial.print("Alarm set to: "); Serial.print(hour); Serial.print(":"); Serial.println(minute);
        Serial.flush();
    }

    long getTime() {
        RtcDateTime now = RTC.GetDateTime();
        return (long) ((now.Hour() * 10000l) + now.Minute() * 100l + now.Second());
    }

    uint8_t checkAlarm() {
        uint8_t alarm = 0;

        if (alarmTriggered) {
            alarmTriggered = false;

            DS3231AlarmFlag flag = RTC.LatchAlarmsTriggeredFlags();

            if (flag & DS3231AlarmFlag_Alarm1) {
                alarm = 1;
            } else if (flag & DS3231AlarmFlag_Alarm2) {
                alarm = 2;
            } else {
                alarm = 3;  // Fake alarm, manually triggered
            }
        }

        return alarm;
    }

    void triggerAlarm() {
        alarmTriggered = true;
    }

};

static ClockClass Clock;

#endif
