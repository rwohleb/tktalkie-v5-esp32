#include "BleSerial.h"

#include <NimBLEDevice.h>

BleSerial tkBleSerial;

static NimBLECharacteristic *txrx = nullptr;

class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer *server, NimBLEConnInfo &info) override {
      tkBleSerial.setConnected(true);
    }
    void onDisconnect(NimBLEServer *server, NimBLEConnInfo &info, int reason) override {
      // Advertising restarts by itself (advertiseOnDisconnect).
      tkBleSerial.setConnected(false);
    }
};

class CharCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic *chr, NimBLEConnInfo &info) override {
      NimBLEAttValue value = chr->getValue();
      tkBleSerial.receive(value.data(), value.length());
    }
};

void BleSerial::begin(unsigned long baud) {
  (void)baud;
  if (started) {
    return;
  }
  started = true;

  NimBLEDevice::init(TK_BLE_NAME);
  NimBLEServer *server = NimBLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());
  server->advertiseOnDisconnect(true);

  NimBLEService *service = server->createService("FFE0");
  txrx = service->createCharacteristic(
      "FFE1",
      NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE |
      NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::NOTIFY);
  txrx->setCallbacks(new CharCallbacks());

  NimBLEAdvertising *adv = NimBLEDevice::getAdvertising();
  adv->addServiceUUID("FFE0");
  adv->setName(TK_BLE_NAME);
  adv->enableScanResponse(true);
  adv->start();
}

bool BleSerial::connected() const {
  return isConnected;
}

void BleSerial::setConnected(bool on) {
  isConnected = on;
  if (!on) {
    txLen = 0;
  }
}

void BleSerial::receive(const uint8_t *data, size_t len) {
  portENTER_CRITICAL(&rxLock);
  for (size_t i = 0; i < len; i++) {
    size_t next = (rxHead + 1) % RX_SIZE;
    if (next == rxTail) {
      break;  // full: drop the rest, as a UART would overrun
    }
    rx[rxHead] = data[i];
    rxHead = next;
  }
  portEXIT_CRITICAL(&rxLock);
}

int BleSerial::available() {
  // V5 polls this every loop, so it doubles as the flush for output that
  // stopped short of a full packet.
  if (txLen > 0 && millis() - txLastWrite >= TX_IDLE_MS) {
    flush();
  }
  portENTER_CRITICAL(&rxLock);
  int n = (rxHead + RX_SIZE - rxTail) % RX_SIZE;
  portEXIT_CRITICAL(&rxLock);
  return n;
}

int BleSerial::read() {
  int c = -1;
  portENTER_CRITICAL(&rxLock);
  if (rxHead != rxTail) {
    c = rx[rxTail];
    rxTail = (rxTail + 1) % RX_SIZE;
  }
  portEXIT_CRITICAL(&rxLock);
  return c;
}

int BleSerial::peek() {
  int c = -1;
  portENTER_CRITICAL(&rxLock);
  if (rxHead != rxTail) {
    c = rx[rxTail];
  }
  portEXIT_CRITICAL(&rxLock);
  return c;
}

void BleSerial::flush() {
  if (txLen == 0) {
    return;
  }
  if (isConnected && txrx != nullptr) {
    // The host stack runs out of buffers when V5 sends a file a byte at a
    // time (sendConfig); wait for it to drain rather than drop data.
    for (int tries = 0; tries < 50; tries++) {
      if (txrx->notify(tx, txLen)) {
        break;
      }
      delay(5);
    }
  }
  txLen = 0;
}

size_t BleSerial::write(uint8_t c) {
  if (!isConnected) {
    return 1;  // like the HM-10, output with nobody connected goes nowhere
  }
  tx[txLen++] = c;
  txLastWrite = millis();
  if (txLen == TX_CHUNK || c == '\n') {
    flush();
  }
  return 1;
}
