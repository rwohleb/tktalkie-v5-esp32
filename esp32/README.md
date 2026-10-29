# TK-Talkie V5 for ESP32

This folder, plus `platformio.ini` at the repo root, holds everything needed
to build the V5 firmware (at the repo root) for ESP32. The V5 sources are
compiled as-is; the files here stand in for the Teensy-specific pieces.

| Path | Replaces |
|---|---|
| `tktalkie5_ino.cpp` | Builds `tktalkie5.ino` as plain C++ (PlatformIO's `.ino` converter is unreliable here) |
| `lib/TKPort/tk_port.h` | Force-included ahead of V5. Board I2S pins, `Serial1` → BLE, `analogRead` pin mapping, `LED_BUILTIN`, `elapsedMillis` |
| `lib/TKPort/AudioControlSGTL5000.*` | The SGTL5000 control class, driving the ES8388 |
| `lib/TKPort/BleSerial.*` | The HM-10 BLE module |
| `lib/TKPort/tk_board.h` | Pin map per board |
| `lib/SD` | Teensy SD library, backed by SD_MMC |
| `lib/Snooze` | Teensy Snooze library (`usb` serial, light sleep) |
| `lib/SerialFlash` | Empty; V5 includes it but doesn't use it |

## Boards

| Environment | Board |
|---|---|
| `audiokit` | Ai-Thinker ESP32-Audio-Kit V2.2 (ESP32-A1S, ES8388) |
| `teensy40` | The original Teensy 4.0 target, built from the same sources to check that V5 edits made for ESP32 still compile there |

## Building

From the repo root (or open the repo folder in VS Code with the PlatformIO extension). Build from PowerShell or VS Code, not Git Bash: the ESP-IDF tool installer refuses to run under MSYS.

```bash
pio run -e audiokit
```

Upload and open the serial monitor:

```bash
pio run -e audiokit -t upload -t monitor
```

## Pulling in upstream V5 changes

```bash
git fetch upstream
git merge upstream/master
```
