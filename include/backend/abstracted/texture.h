#pragma once
#include <cstddef>
#include <cstdlib>
#include "positions.h"

struct Texture {
    Position position;
    Size size;
    void* handle;
};

struct Rect {
    Position position;
    Size size;
};
