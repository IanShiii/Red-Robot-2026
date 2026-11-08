#include "Autons.h"
#include "RedRobot.h"

void Autons::Auton1::Start() {
    // drive forward for 1 second
    RedRobot::WriteDCMotor(2, 30);
    RedRobot::WriteDCMotor(3, 30);
    delay(1000);

    // stop the motors
    RedRobot::WriteDCMotor(2, 0);
    RedRobot::WriteDCMotor(3, 0);
    delay(1000);

    // turn left 90 degrees
    RedRobot::WriteDCMotor(2, -30);
    RedRobot::WriteDCMotor(3, 30);
    delay(800);

    // stop the motors
    RedRobot::WriteDCMotor(2, 0);
    RedRobot::WriteDCMotor(3, 0);
    delay(1000);

    // drive forward
    RedRobot::WriteDCMotor(2, 30);
    RedRobot::WriteDCMotor(3, 30);
    delay(3000);

    // stop the motors
    RedRobot::WriteDCMotor(2, 0);
    RedRobot::WriteDCMotor(3, 0);
    delay(1000);

    // turn left slightly
    RedRobot::WriteDCMotor(2, -30);
    RedRobot::WriteDCMotor(3, 30);
    delay(500);

    // stop the motors
    RedRobot::WriteDCMotor(2, 0);
    RedRobot::WriteDCMotor(3, 0);
    delay(1000);

    // drive forward
    RedRobot::WriteDCMotor(2, 30);
    RedRobot::WriteDCMotor(3, 30);
    delay(2000);

    // stop the motors
    RedRobot::WriteDCMotor(2, 0);
    RedRobot::WriteDCMotor(3, 0);
}

void Autons::Auton1::Loop() {
    return;
}

void Autons::LineFollowing::FollowLine() {
    uint16_t values[6];
    RedRobot::ReadLineSensor(values);

    if (RedRobot::IsRemoteControlled()) {
        return;
    }

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
