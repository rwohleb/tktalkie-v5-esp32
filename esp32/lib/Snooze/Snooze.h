// Stand-in for the Teensy Snooze library, covering what V5 uses:
//   SnoozeUSBSerial usb;            -> the ESP32 Serial port
//   SnoozeDigital  snoozeDigital;   -> wake-up pins
//   Snooze.deepSleep(block);        -> ESP32 light sleep
// Light sleep returns to the caller on wake-up, like Snooze does on the
// Teensy, so V5's wake-button logic in gotoSleep() runs as written.
#pragma once

#include <Arduino.h>

class SnoozeUSBSerial : public Stream {
  public:
    int available() override { ensureBegun(); return Serial.available(); }
    int read() override { ensureBegun(); return Serial.read(); }
    int peek() override { ensureBegun(); return Serial.peek(); }
    void flush() override { Serial.flush(); }
    size_t write(uint8_t c) override { ensureBegun(); return Serial.write(c); }
    size_t write(const uint8_t *buf, size_t len) override { ensureBegun(); return Serial.write(buf, len); }
    using Print::write;

  private:
    // V5 never calls begin(): Teensy USB serial is always up
    void ensureBegun() {
        if (!begun_) {
            Serial.begin(115200);
            begun_ = true;
        }
    }
    bool begun_ = false;
};

class SnoozeDigital {
  public:
    static constexpr int MAX_PINS = 8;

    void pinMode(int pin, int mode, int type);

    int count() const { return count_; }
    int pin(int i) const { return pins_[i]; }

  private:
    int pins_[MAX_PINS];
    int count_ = 0;
};

class SnoozeBlock {
  public:
    SnoozeBlock(SnoozeUSBSerial &usb, SnoozeDigital &digital) : digital_(digital) { (void)usb; }
    SnoozeDigital &digital() { return digital_; }

  private:
    SnoozeDigital &digital_;
};

class SnoozeClass {
  public:
    // Returns the pin that woke the board, or -1
    int deepSleep(SnoozeBlock &block);
};

extern SnoozeClass Snooze;
