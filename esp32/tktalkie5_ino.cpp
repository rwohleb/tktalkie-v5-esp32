// Builds V5's sketch as plain C++ instead of through PlatformIO's .ino
// converter, which can't find the toolchain on Windows with this platform.
// tk_port.h (force-included) already brings in Arduino.h.
#include "../tktalkie5.ino"
