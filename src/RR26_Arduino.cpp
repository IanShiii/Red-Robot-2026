/**
 * @brief Red Robot 2026 main code!
 * 
 * Read the comments below to figure out how to get started!
 */

// Libraries with driver code (don't remove these!)
#include <Arduino.h>                // make sure this is first
#include "RedRobot.h"
#include "LineFollowing.h"

// Declare some global variables
bool started; // boolean (true or false) to keep track of if we should start
int counter;  // integer to hold a count value

// This function runs ONCE, when the microcontroller boots up
void setup() {
    // Initialize all components on the robot before doing any work
  RedRobot::InitRedRobot();         // call this first
  RedRobot::InitDCMotors();
  RedRobot::InitServo();
  RedRobot::InitLineSensors();
  RedRobot::InitAuton();            // call this to activate the AutonomousLoop

  // Initialize our variables
  started = false;
  counter = 0;
}

enum class AutonMode {
  None,
  LineFollowToTrenchZone,
  LineFollowToButtonZone
};

AutonMode currentAutonMode = AutonMode::LineFollowToButtonZone;

static constexpr int LINE_FOLLOW_TO_TRENCH_ZONE_COUNTERS = 230;
static constexpr int LINE_FOLLOW_TO_BUTTON_ZONE_COUNTERS = 350;

// This function runs over and over (in a loop) when you are in autonomous mode.
// As an example, this code moves out of the starting box at start, and 
// Look at RedRobot.h to see the robot interface: what functions you can call and how.
void AutonomousLoop() {

    if (!started) {
        started = true;
        // Autons::Auton1::Start();
    }

    if (currentAutonMode == AutonMode::LineFollowToTrenchZone) {
        if (counter < LINE_FOLLOW_TO_TRENCH_ZONE_COUNTERS) {
            LineFollowing::FollowLine();
        } else {
            RedRobot::WriteDCMotor(2, 0);
            RedRobot::WriteDCMotor(3, 0);
            return;
        }
    }

    if (currentAutonMode == AutonMode::LineFollowToButtonZone) {
        if (counter < LINE_FOLLOW_TO_BUTTON_ZONE_COUNTERS) {
            LineFollowing::FollowLine();
        } else {
            RedRobot::WriteDCMotor(2, 0);
            RedRobot::WriteDCMotor(3, 0);
            return;
        }
    }
    
    // Print to the serial console to debug!
    Serial.print("[AUTO] Running ");
    Serial.print(counter);
    Serial.println();

    // increment the counter
    counter = counter + 1;
}
