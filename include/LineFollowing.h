#pragma once

#include "Arduino.h"

namespace LineFollowing {
    constexpr int weights[6] = {-5, -3, -1, 1, 3, 5};
    // Black detection thresholds
    constexpr uint16_t thresholds[6] = {
        6451, 5362, 4747, 4752, 4724, 4544
    };
    constexpr int kP = 6;

    void FollowLine();
}
