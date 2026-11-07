#include <Arduino.h>
#include "RedRobot.h"

// Black detection thresholds
const uint16_t threshold[6] = {
  6451, 5362, 4747, 4752, 4724, 4544
};

// Active sensor order: left to right
const uint8_t leftToRight[6] = {0, 1, 2, 3, 4, 5};
const int weights[6] = {-5, -3, -1, 1, 3, 5};

const int LEFT_MOTOR = 2;
const int RIGHT_MOTOR = 3;

const int BASE_SPEED = 25;
const int MAX_SPEED = 45;
const float KP = 6.0f;

bool finished = false;
unsigned long lastPrint = 0;

void stopWheels() {
  RedRobot::WriteDCMotor(LEFT_MOTOR, 0);
  RedRobot::WriteDCMotor(RIGHT_MOTOR, 0);
}

void setup() {
  RedRobot::InitRedRobot();
  RedRobot::InitDCMotors();
  RedRobot::InitLineSensors();

  stopWheels();

  Serial.println("Starting in 5 seconds...");
  delay(5000);

  RedRobot::InitAuton();
  Serial.println("Following line.");
}

void AutonomousLoop() {
  // Stay stopped until reset
  if (finished) {
    delay(10);
    return;
  }

  uint16_t values[6];
  RedRobot::ReadLineSensor(values);

  if (RedRobot::IsRemoteControlled()) {
    return;
  }

  int blackCount = 0;
  int weightedSum = 0;
  bool black[6];

  for (int i = 0; i < 6; i++) {
    int channel = leftToRight[i];
    black[i] = values[channel] > threshold[channel];

    if (black[i]) {
      blackCount++;
      weightedSum += weights[i];
    }
  }

  // Stop when all sensors lose the line
  if (blackCount == 0) {
    stopWheels();
    finished = true;
    Serial.println("Line: ......  STOPPED");
    Serial.println("Press RESET to restart.");
    return;
  }

  int leftSpeed;
  int rightSpeed;

  if (blackCount == 6) {
    // Move slowly through all-black areas
    leftSpeed = 20;
    rightSpeed = 20;
  } else {
    // Steer toward the line
    float error = (float)weightedSum / blackCount;
    int correction = (int)(KP * error);

    leftSpeed = constrain(
      BASE_SPEED + correction, 0, MAX_SPEED
    );
    rightSpeed = constrain(
      BASE_SPEED - correction, 0, MAX_SPEED
    );
  }

  RedRobot::WriteDCMotor(LEFT_MOTOR, leftSpeed);
  RedRobot::WriteDCMotor(RIGHT_MOTOR, rightSpeed);

  // Print status every 200 ms
  if (millis() - lastPrint >= 200UL) {
    lastPrint = millis();

    Serial.print("Line: ");
    for (int i = 0; i < 6; i++) {
      Serial.print(black[i] ? '#' : '.');
    }

    Serial.print("  Left: ");
    Serial.print(leftSpeed);
    Serial.print("  Right: ");
    Serial.println(rightSpeed);
  }

  delay(5);
}