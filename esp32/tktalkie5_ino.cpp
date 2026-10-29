// Builds V5's sketch as plain C++ instead of through PlatformIO's .ino
// converter, which can't find the toolchain on Windows with this platform.
// tk_port.h is force-included already; naming it here as well lets
// PlatformIO's dependency scanner see it, so edits to the port layer
// rebuild the sketch.
#include "tk_port.h"
#include "../tktalkie5.ino"
