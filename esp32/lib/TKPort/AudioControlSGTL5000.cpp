#include "AudioControlSGTL5000.h"

#include "tk_board.h"

AudioControlSGTL5000::AudioControlSGTL5000()
    : codec_({TK_PIN_I2C_SDA, TK_PIN_I2C_SCL, TK_PIN_AMP, TK_AMP_ACTIVE}) {}

bool AudioControlSGTL5000::enable() { return codec_.enable(); }

bool AudioControlSGTL5000::disable() { return codec_.disable(); }

bool AudioControlSGTL5000::volume(float level) { return codec_.volume(level); }

bool AudioControlSGTL5000::inputSelect(int n) { return codec_.inputSelect(n); }

bool AudioControlSGTL5000::micGain(unsigned int dB) {
    // TODO(phase 3): map to the ES8388 PGA (0 - 24 dB) properly
    if (dB > 63) dB = 63;
    return codec_.inputLevel(dB / 63.0f);
}

bool AudioControlSGTL5000::lineInLevel(uint8_t level) {
    // TODO(phase 3): map to the ES8388 PGA
    if (level > 15) level = 15;
    return codec_.inputLevel(level / 15.0f);
}

bool AudioControlSGTL5000::lineOutLevel(uint8_t level) {
    // TODO(phase 3): the ES8388 has one output volume; decide how this
    // combines with volume()
    (void)level;
    return true;
}

bool AudioControlSGTL5000::muteHeadphone() { headphoneMuted_ = true; return updateMute(); }

bool AudioControlSGTL5000::unmuteHeadphone() { headphoneMuted_ = false; return updateMute(); }

bool AudioControlSGTL5000::muteLineout() { lineoutMuted_ = true; return updateMute(); }

bool AudioControlSGTL5000::unmuteLineout() { lineoutMuted_ = false; return updateMute(); }

// The ES8388's headphone and line outputs share one DAC, so it is only muted
// when V5 has muted both.
bool AudioControlSGTL5000::updateMute() {
    return (headphoneMuted_ && lineoutMuted_) ? codec_.mute() : codec_.unmute();
}

// TODO(phase 3): software EQ (two AudioFilterBiquad between voiceMixer and
// i2s2) and high-pass filter. Until then these only accept the calls.
bool AudioControlSGTL5000::audioPostProcessorEnable() { return true; }

bool AudioControlSGTL5000::adcHighPassFilterEnable() { return true; }

bool AudioControlSGTL5000::adcHighPassFilterDisable() { return true; }

bool AudioControlSGTL5000::eqSelect(int mode) { (void)mode; return true; }

bool AudioControlSGTL5000::eqBands(float b1, float b2, float b3, float b4, float b5) {
    (void)b1; (void)b2; (void)b3; (void)b4; (void)b5;
    return true;
}
