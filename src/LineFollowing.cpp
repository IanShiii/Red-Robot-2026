#include "LineFollowing.h"
#include "RedRobot.h"

void LineFollowing::FollowLine() {
    uint16_t values[6];
    RedRobot::ReadLineSensor(values);

    int blackCount = 0;
    int weightedSum = 0;
    bool black[6];

    for (int i = 0; i < 6; i++) {
        int channel = i;
        black[i] = values[channel] > thresholds[channel];

        if (black[i]) {
            blackCount++;
            weightedSum += weights[i];
        }
    }

    int leftSpeed;
    int rightSpeed;

    if (blackCount == 6 || blackCount == 0) {
        leftSpeed = 20;
        rightSpeed = 20;
    } else {
        // Steer toward the line
        float error = (float)weightedSum / blackCount;
        int correction = (int)(kP * error);

        leftSpeed = constrain(25 + correction, 0, 100);
        rightSpeed = constrain(25 - correction, 0, 100);
    }

    RedRobot::WriteDCMotor(2, leftSpeed);
    RedRobot::WriteDCMotor(3, rightSpeed);
}
