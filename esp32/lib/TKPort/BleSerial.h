// Replacement for the HM-10 module V5 talks to on Serial1: a NimBLE
// peripheral with the HM-10's service FFE0 and characteristic FFE1.
// Writes from the app land in a receive buffer; output goes out as
// 20-byte notifications, the HM-10's packet size.
//
// Like the HM-10, connecting and disconnecting are invisible to V5; it
// tracks the app through its own connect/disconnect commands.
#pragma once

#include <Arduino.h>

// Advertised name. The app lists every nearby BLE device rather than
// looking for a particular name. (The original build shows up under its
// module's factory name, "HMSoft" for an HM-10.)
// Override with -D TK_BLE_NAME=\"...\" in platformio.ini.
#ifndef TK_BLE_NAME
#define TK_BLE_NAME "TK-Talkie"
#endif

class BleSerial : public Stream {
  public:
    // The baud rate only mattered for the HM-10's UART; it is ignored.
    void begin(unsigned long baud);
    bool connected() const;

    int available() override;
    int read() override;
    int peek() override;
    void flush() override;
    size_t write(uint8_t c) override;
    using Print::write;

    // Called from the NimBLE task.
    void receive(const uint8_t *data, size_t len);
    void setConnected(bool on);

  private:
    static const size_t RX_SIZE = 512;
    static const size_t TX_CHUNK = 20;
    static const uint32_t TX_IDLE_MS = 10;

    uint8_t rx[RX_SIZE];
    volatile size_t rxHead = 0;
    volatile size_t rxTail = 0;
    portMUX_TYPE rxLock = portMUX_INITIALIZER_UNLOCKED;

    uint8_t tx[TX_CHUNK];
    size_t txLen = 0;
    uint32_t txLastWrite = 0;

    volatile bool isConnected = false;
    bool started = false;
};

extern BleSerial tkBleSerial;
