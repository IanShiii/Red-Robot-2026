#include "RedRobot.h"

bool useTankDrive = false;

namespace Bluefruit
{
  /* DPad_Pressed is called everytime a D-Pad button is pressed or released.
   *
   * Only activates after RedRobot::StartRemoteControl() is called.
   * Button is one of {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3}
   */
  void DPad_Pressed(button_t Button, bool buttonPressedDown)
  {
    if (!RedRobot::IsRemoteControlled())
      return;

    // values stay the same between calls to DPad_Pressed
    static bool button_pressed[4] = {false, false, false, false};

    button_pressed[Button] = (buttonPressedDown == true);

    // TODO: edit your speed output to the yellow motors below!
    // stop = 0, slow = 50, fast = 100
    // range [-100,100]

    int left_speed = 0;
    int right_speed = 0;

    if (button_pressed[UP])
    { // Forward
      left_speed += 100;
      right_speed += 100;
    }
    if (button_pressed[DOWN])
    { // Backward
      left_speed -= 100;
      right_speed -= 100;
    }
    if (button_pressed[LEFT])
    { // Turn left_speed
      left_speed -= 50;
      right_speed += 50;
    }
    if (button_pressed[RIGHT])
    { // Turn right_speed
      left_speed += 50;
      right_speed -= 50;
    }

    // Clamp and write speeds
    RedRobot::WriteDCMotor(2, constrain(left_speed, -100, 100));
    RedRobot::WriteDCMotor(3, constrain(right_speed, -100, 100));
  }

  void Button1_Pressed(bool buttonPressedDown)
  {
    if (!RedRobot::IsRemoteControlled() && buttonPressedDown)
    {
      RedRobot::StartRemoteControl();

      // start remote controlled with the DC motors off
      RedRobot::WriteDCMotor(2, 0);
      RedRobot::WriteDCMotor(3, 0);
    }
  }

  void Button2_Pressed(bool buttonPressedDown)
  {
    // Ex: move an arm when the button is pressed
    if (buttonPressedDown)
    {
      RedRobot::SetServo(0, 180);
      RedRobot::SetServo(1, 180);
      RedRobot::SetServo(2, 180);
      RedRobot::SetServo(3, 180);
    }
    else
    {
      RedRobot::SetServo(0, 0);
      RedRobot::SetServo(1, 0);
      RedRobot::SetServo(2, 0);
      RedRobot::SetServo(3, 0);
    }
  }

  void Button3_Pressed(bool buttonPressedDown)
  {
  }

  void Button4_Pressed(bool buttonPressedDown)
  {
  }
}

namespace RR_Controller
{
  static int8_t leftX = 0;
  static int8_t leftY = 0;
  static int8_t rightX = 0;
  static int8_t rightY = 0;

  static constexpr float JOYSTICK_DEADZONE = 5.0;
  static constexpr float MIN_EFFECTIVE_SPEED = 25.0;
  static constexpr float RESPONSE_CURVE_EXPONENT = 1.5;

  static constexpr float ARCADE_TURN_SPEED_SCALE = 0.5; // Scale down turning speed for arcade drive to make it easier to control

  void UpdateDrive() {
    if (!RedRobot::IsRemoteControlled())
      return;
    
    // Apply deadzone
    int8_t shaped_leftX = (abs(leftX) < JOYSTICK_DEADZONE) ? 0 : leftX;
    int8_t shaped_leftY = (abs(leftY) < JOYSTICK_DEADZONE) ? 0 : leftY;
    int8_t shaped_rightX = (abs(rightX) < JOYSTICK_DEADZONE) ? 0 : rightX;
    int8_t shaped_rightY = (abs(rightY) < JOYSTICK_DEADZONE) ? 0 : rightY;

    // Apply exponential response shaping
    shaped_leftX = pow(shaped_leftX / 100.0, RESPONSE_CURVE_EXPONENT) * 100 * (shaped_leftX < 0 ? -1 : 1);
    shaped_leftY = pow(shaped_leftY / 100.0, RESPONSE_CURVE_EXPONENT) * 100 * (shaped_leftY < 0 ? -1 : 1);
    shaped_rightX = pow(shaped_rightX / 100.0, RESPONSE_CURVE_EXPONENT) * 100 * (shaped_rightX < 0 ? -1 : 1);
    shaped_rightY = pow(shaped_rightY / 100.0, RESPONSE_CURVE_EXPONENT) * 100 * (shaped_rightY < 0 ? -1 : 1);

    int16_t leftWheelSpeed = 0;
    int16_t rightWheelSpeed = 0;

    if (useTankDrive) {
      leftWheelSpeed = shaped_leftY;
      rightWheelSpeed = shaped_rightY;

      // Normalize to [-100, 100]
      int maxMagnitude = max(abs(leftWheelSpeed), abs(rightWheelSpeed));

      if (maxMagnitude > 100) {
        leftWheelSpeed = leftWheelSpeed * 75 / maxMagnitude;
        rightWheelSpeed = rightWheelSpeed * 75 / maxMagnitude;
      }
    } else {
      leftWheelSpeed = leftY + rightX * ARCADE_TURN_SPEED_SCALE;
      rightWheelSpeed = leftY - rightX * ARCADE_TURN_SPEED_SCALE;

      int maxMagnitude = max(abs(leftWheelSpeed), abs(rightWheelSpeed));
      if (maxMagnitude > 100) {
        leftWheelSpeed = leftWheelSpeed * 100 / maxMagnitude;
        rightWheelSpeed = rightWheelSpeed * 100 / maxMagnitude;
      }
    }

    // Apply minimum effective speed threshold
    if (abs(leftWheelSpeed) < MIN_EFFECTIVE_SPEED) {
      leftWheelSpeed = 0;
    }
    if (abs(rightWheelSpeed) < MIN_EFFECTIVE_SPEED) {
      rightWheelSpeed = 0;
    }

    RedRobot::WriteDCMotor(2, leftWheelSpeed);
    RedRobot::WriteDCMotor(3, rightWheelSpeed);
  }

  void Joystick1(int8_t x, int8_t y)
  {
    leftX = x;
    leftY = y;
    UpdateDrive();
  }

  void Joystick2(int8_t x, int8_t y)
  {
    rightX = x;
    rightY = y;
    UpdateDrive();
  }

  /* DPad_Pressed is called everytime a D-Pad button is pressed or released.
   *
   * Only activates after RedRobot::StartRemoteControl() is called.
   * Button is one of {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, CENTER = 4}
   */
  void DPad_Pressed(button_t Button, bool buttonPressedDown)
  {
    // values stay the same between calls to DPad_Pressed
    static bool button_pressed[5] = {false, false, false, false, false};

    button_pressed[Button] = (buttonPressedDown == true);

    // TODO: edit your speed output to the yellow motors below!
    // stop = 0, slow = 50, fast = 100
    // range [-100,100]

    int left_speed = 0;
    int right_speed = 0;

    if (button_pressed[UP])
    { // Forward
      left_speed += 100;
      right_speed += 100;
    }
    if (button_pressed[DOWN])
    { // Backward
      left_speed -= 100;
      right_speed -= 100;
    }
    if (button_pressed[LEFT])
    { // Turn left_speed
      left_speed -= 50;
      right_speed += 50;
    }
    if (button_pressed[RIGHT])
    { // Turn right_speed
      left_speed += 50;
      right_speed -= 50;
    }

    // Clamp and write speeds
    RedRobot::WriteDCMotor(2, constrain(left_speed, -100, 100));
    RedRobot::WriteDCMotor(3, constrain(right_speed, -100, 100));
  }

  void ButtonX_Pressed(bool buttonPressedDown)
  {
    if (!RedRobot::IsRemoteControlled() && buttonPressedDown)
    {
      RedRobot::StartRemoteControl();

      // start remote controlled with the DC motors off
      RedRobot::WriteDCMotor(2, 0);
      RedRobot::WriteDCMotor(3, 0);
    }
  }

  void ButtonY_Pressed(bool buttonPressedDown)
  {
    Serial.printf("Y pressed\n");
    if (buttonPressedDown)
      useTankDrive = !useTankDrive;
  }

  void ButtonB_Pressed(bool buttonPressedDown)
  {
  }

  void ButtonA_Pressed(bool buttonPressedDown)
  {
  }
}
