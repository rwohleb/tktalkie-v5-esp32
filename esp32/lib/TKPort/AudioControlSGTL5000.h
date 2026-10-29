// Stand-in for the Teensy Audio Library's SGTL5000 control class, driving the
// board's ES8388 instead. Keeps the method names and value ranges V5 uses, so
// globals.h's `AudioControlSGTL5000 audioShield;` works unchanged.
#pragma once

#include <Audio.h>

// eqSelect() modes, same values as the Teensy library
#define FLAT_FREQUENCY       0
#define PARAMETRIC_EQUALIZER 1
#define TONE_CONTROLS        2
#define GRAPHIC_EQUALIZER    3

class AudioControlSGTL5000 {
  public:
    AudioControlSGTL5000();

    bool enable();
    bool disable();

    bool volume(float level);              // 0.0 - 1.0
    bool inputSelect(int n);               // AUDIO_INPUT_LINEIN / AUDIO_INPUT_MIC
    bool micGain(unsigned int dB);         // SGTL5000: 0 - 63 dB
    bool lineInLevel(uint8_t level);       // SGTL5000: 0 - 15
    bool lineOutLevel(uint8_t level);      // SGTL5000: 13 - 31 (13 = loudest)

    bool muteHeadphone();
    bool unmuteHeadphone();
    bool muteLineout();
    bool unmuteLineout();

    bool audioPostProcessorEnable();
    bool adcHighPassFilterEnable();
    bool adcHighPassFilterDisable();
    bool eqSelect(int mode);
    bool eqBands(float b1, float b2, float b3, float b4, float b5);  // each -1.0 - +1.0

  private:
    bool updateMute();

    AudioControlES8388 codec_;
    bool headphoneMuted_ = false;
    bool lineoutMuted_ = false;
};
