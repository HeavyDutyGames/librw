#pragma once

#include "rwbgfx.h"

namespace rw {
namespace bgfx {

struct Globals {
    bool initialized;
    uint16 width;
    uint16 height;
    uint16 viewId;
    uint64 state;
};

extern Globals bgfxGlobals;

} // namespace bgfx
} // namespace rw

