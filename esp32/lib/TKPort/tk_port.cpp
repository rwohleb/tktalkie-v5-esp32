#include <Arduino.h>
#include <esp_random.h>

#include "tk_board.h"

int tk_analogRead(uint8_t teensyPin) {
    switch (teensyPin) {
    case 15: // volume pot
        // V5 expects 10-bit readings (0-1023)
        return analogRead(TK_PIN_VOLUME) >> 2;
    case 0:
        // randomSeed(analogRead(0)): GPIO0 is the codec MCLK, never touch it
        return (int)(esp_random() & 0x3FF);
    default:
        return 0;
    }
}
