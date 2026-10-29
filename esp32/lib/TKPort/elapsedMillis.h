// Teensy's elapsedMillis: a variable that counts up in milliseconds.
#pragma once
#include <Arduino.h>

class elapsedMillis {
  public:
    elapsedMillis() : start_(millis()) {}
    elapsedMillis(unsigned long val) : start_(millis() - val) {}
    operator unsigned long() const { return millis() - start_; }
    elapsedMillis &operator=(unsigned long val) { start_ = millis() - val; return *this; }
    elapsedMillis &operator-=(unsigned long val) { start_ += val; return *this; }
    elapsedMillis &operator+=(unsigned long val) { start_ -= val; return *this; }

  private:
    unsigned long start_;
};
