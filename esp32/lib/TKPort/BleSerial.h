// Replacement for the HM-10 module V5 talks to on Serial1.
// TODO(phase 4): NimBLE peripheral with service FFE0 / characteristic FFE1.
// Until then it is a Stream that drops output and never has input.
#pragma once

#include <Arduino.h>

class BleSerial : public Stream {
  public:
    void begin(unsigned long baud) { (void)baud; }

    int available() override { return 0; }
    int read() override { return -1; }
    int peek() override { return -1; }
    void flush() override {}
    size_t write(uint8_t c) override { (void)c; return 1; }
    using Print::write;
};

extern BleSerial tkBleSerial;
