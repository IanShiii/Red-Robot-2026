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
