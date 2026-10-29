// Pin map for the selected board (chosen by a -D TK_BOARD_* build flag).
#pragma once

#if defined(TK_BOARD_AUDIOKIT)

// Ai-Thinker ESP32-Audio-Kit V2.2 (ESP32-A1S, ES8388)
// I2S, codec and SD pins match ESP-beatsy's esp32-audio-kit environment.
#define TK_PIN_I2S_BCLK  27
#define TK_PIN_I2S_WS    25
#define TK_PIN_I2S_DOUT  26
#define TK_PIN_I2S_DIN   35
#define TK_PIN_I2S_MCLK  0

#define TK_PIN_I2C_SDA   33
#define TK_PIN_I2C_SCL   32

#define TK_PIN_AMP       21   // onboard speaker amp enable
#define TK_AMP_ACTIVE    true

#define TK_PIN_SD_CLK    14
#define TK_PIN_SD_CMD    15
#define TK_PIN_SD_D0     2
#define TK_PIN_SD_D1     4
#define TK_PIN_SD_D2     12
#define TK_PIN_SD_D3     13

#define TK_PIN_LED       22   // TODO: confirm against the board schematic
#define TK_PIN_VOLUME    34   // volume pot (ADC1, input only); V5 reads Teensy pin 15

#else
#error "No board selected: add -D TK_BOARD_<name> to the environment's build_flags"
#endif
