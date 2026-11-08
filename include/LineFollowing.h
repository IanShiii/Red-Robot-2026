#pragma once

#include "Arduino.h"

namespace LineFollowing {
    constexpr int weights[6] = {-5, -3, -1, 1, 3, 5};
    // Black detection thresholds
    constexpr uint16_t thresholds[6] = {
        9000, 9000, 9000, 9000, 9000, 9000
    };
    constexpr int kP = 6;

    void FollowLine();
}
