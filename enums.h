#ifndef ENUMS_H
#define ENUMS_H

#include "motorstate.h"
#include "direction.h"

enum{
    BACK_FLOOR,
    FRONT_FLOOR
};

enum class CameraMove{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STOP,
    CameraMoveNumber
};

#endif // ENUMS_H

