#include "SD.h"

#include <tk_board.h>

SDClass SD;

bool SDClass::begin(uint8_t csPin) {
    (void)csPin;
    if (!SD_MMC.setPins(TK_PIN_SD_CLK, TK_PIN_SD_CMD, TK_PIN_SD_D0, TK_PIN_SD_D1, TK_PIN_SD_D2, TK_PIN_SD_D3)) {
        return false;
    }
    return SD_MMC.begin("/sdcard", false);  // 4-bit mode
}

File SDClass::open(const char *path, int mode) {
    // Teensy's FILE_WRITE creates the file and appends. V5 deletes a file
    // before rewriting it, so append matches its behavior.
    return SD_MMC.open(path, mode == FILE_WRITE ? "a" : "r");
}
