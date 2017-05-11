#ifndef __SERIALCLI__
#define __SERIALCLI__

#include <inttypes.h>
#include <Arduino.h>

#define EEPROM_PAGE_SIZE 32

enum ManagerState {
    IDLE      = 1 << 0,
    MENU      = 1 << 1,
    SET       = 1 << 2,
    TIME      = 1 << 3,
    DATE      = 1 << 4,
    ALARM_ONE = 1 << 5,
    ALARM_TWO = 1 << 6,
    WEIGHT    = 1 << 7,
    SCALE     = 1 << 8
};

inline ManagerState operator~(ManagerState a) {
    return static_cast<ManagerState>(~static_cast<uint8_t>(a));
}

inline ManagerState operator|(ManagerState a, ManagerState b) {
    return static_cast<ManagerState>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline ManagerState operator&(ManagerState a, ManagerState b) {
    return static_cast<ManagerState>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

typedef void (*Handler)(ManagerState trigger, uint16_t value);

class Cerial {

public:
    Cerial(Handler handler);
    void add(uint8_t method, uint8_t id, String label);
    void begin();
    void update();

private:
    ManagerState state = IDLE;
    Handler handler;

//    struct {
//        uint8_t index;
//        Command* items[COMMANDLINE_COUNT];
//    } commands;

    char buffer[EEPROM_PAGE_SIZE-4];
    uint8_t buffer_length = 0;

};

#endif