// Force-included ahead of every V5 source file (see build_src_flags in
// platformio.ini). Maps the Teensy-specific names V5 uses onto ESP32
// equivalents so the V5 sources build unmodified.
#pragma once

#include <Arduino.h>
#include <Audio.h>

#include "tk_board.h"
#include "elapsedMillis.h"
#include "AudioControlSGTL5000.h"
#include "BleSerial.h"

// I2S objects with this board's pins. V5 declares them without arguments.
class TKAudioOutputI2S : public AudioOutputI2S {
  public:
    TKAudioOutputI2S()
        : AudioOutputI2S(Pins{TK_PIN_I2S_BCLK, TK_PIN_I2S_WS, TK_PIN_I2S_DOUT, TK_PIN_I2S_MCLK}) {}
};
class TKAudioInputI2S : public AudioInputI2S {
  public:
    TKAudioInputI2S()
        : AudioInputI2S(Pins{TK_PIN_I2S_BCLK, TK_PIN_I2S_WS, TK_PIN_I2S_DIN, TK_PIN_I2S_MCLK}) {}
};
#define AudioOutputI2S TKAudioOutputI2S
#define AudioInputI2S  TKAudioInputI2S

// HM-10 on Serial1 -> BLE
#define Serial1 tkBleSerial

// Teensy pin numbers passed to analogRead -> this board's pins
int tk_analogRead(uint8_t teensyPin);
#define analogRead(pin) tk_analogRead(pin)

// The core's LED_BUILTIN can land on an SD or codec pin
#ifdef LED_BUILTIN
#undef LED_BUILTIN
#endif
#define LED_BUILTIN TK_PIN_LED
