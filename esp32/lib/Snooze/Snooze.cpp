#include "Snooze.h"

#include <driver/gpio.h>
#include <esp_sleep.h>

SnoozeClass Snooze;

void SnoozeDigital::pinMode(int pin, int mode, int type) {
    (void)type;  // V5 always asks for FALLING; light sleep wakes on a low level
    ::pinMode(pin, mode);
    for (int i = 0; i < count_; i++) {
        if (pins_[i] == pin) return;
    }
    if (count_ < MAX_PINS) pins_[count_++] = pin;
}

// TODO(phase 5): power down the codec and amp while asleep
int SnoozeClass::deepSleep(SnoozeBlock &block) {
    SnoozeDigital &digital = block.digital();
    if (digital.count() == 0) return -1;

    Serial.flush();
    for (int i = 0; i < digital.count(); i++) {
        gpio_wakeup_enable((gpio_num_t)digital.pin(i), GPIO_INTR_LOW_LEVEL);
    }
    esp_sleep_enable_gpio_wakeup();
    esp_light_sleep_start();

    int woke = -1;
    for (int i = 0; i < digital.count(); i++) {
        gpio_wakeup_disable((gpio_num_t)digital.pin(i));
        if (woke < 0 && digitalRead(digital.pin(i)) == LOW) woke = digital.pin(i);
    }
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_GPIO);
    return woke;
}
