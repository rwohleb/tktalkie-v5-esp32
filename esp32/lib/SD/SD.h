// Stand-in for the Teensy SD library, backed by the board's SD_MMC slot.
// ESP-beatsy's AudioPlaySdWav::play(filename) also reads from SD_MMC, so
// sounds and V5's config files come from the same card.
#pragma once

#include <Arduino.h>
#include <FS.h>
#include <SD_MMC.h>

// Teensy uses integer open modes, and V5 passes them around as int
#undef FILE_READ
#undef FILE_WRITE
#define FILE_READ  0
#define FILE_WRITE 1

class SDClass {
  public:
    // csPin is the Teensy SPI chip select; the SD_MMC pins come from tk_board.h
    bool begin(uint8_t csPin = 0);

    File open(const char *path, int mode = FILE_READ);
    bool exists(const char *path) { return SD_MMC.exists(path); }
    bool remove(const char *path) { return SD_MMC.remove(path); }
    bool mkdir(const char *path) { return SD_MMC.mkdir(path); }
    bool rmdir(const char *path) { return SD_MMC.rmdir(path); }
};

extern SDClass SD;
