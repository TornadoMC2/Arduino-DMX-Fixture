#include <AccelStepper.h>
#include <DMXSerial.h>

#define POSITION_NUM 10
#define ON LOW
#define OFF HIGH

// Constants for motor geometry
const int stepsPerRevolution = 2048;          // Steps for one full revolution
const int stepsFor180 = stepsPerRevolution / 2; // 1024 steps for 180° movement

// Create an AccelStepper instance in FULL4WIRE mode on pins 8, 10, 9, 11.
// In FULL4WIRE mode, disableOutputs() will set the stepper pins to LOW.
AccelStepper stepperMotor(AccelStepper::FULL4WIRE, 8, 10, 9, 11);

// Digital pins for a 10-bit encoded state from switches:
const int SWITCH_PINS[] = { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9 };

// Speed and update settings:
float constantSpeed = 1000.0;          // Steps per second (constant speed)
float maxSpeedVal   = 1000.0;          // Maximum speed
int stepsPerUpdate = 75;              // Maximum number of steps to take per update
unsigned long lastStepUpdate = 0;     // Timestamp of last update (microseconds)
unsigned long stepInterval = 2000;    // Minimum time between update blocks (microseconds)

void setup() {
  DMXSerial.init(DMXReceiver);
  stepperMotor.setMaxSpeed(maxSpeedVal);
  stepperMotor.enableOutputs();
  
  /*
  Serial.begin(250000);
  Serial.println("AccelStepper linear DMXStepper (fast) controller started.");
  */

  // Configure switch pins as inputs with pull-ups
  for (int i = 0; i < POSITION_NUM; i++) {
    pinMode(SWITCH_PINS[i], INPUT_PULLUP);
  }
}

void loop() {
  // Build a 10-bit encoded state from the switch pins.
  int encoded_state = 0;
  for (int i = 0; i < POSITION_NUM; i++) {
    if (digitalRead(SWITCH_PINS[i]) == ON)
      encoded_state |= 1 << i;
  }

  /*
  Serial.print("Current DMX Address: ");
  Serial.println(encoded_state);
  */

  int angle = encoded_state;
  int speed = encoded_state + 1;
  int value = encoded_state + 2;

  // Read DMX value from the channel given by 'value'
  int dmxValue = DMXSerial.read(value);
  Serial.print("Read value: ");
  Serial.println(dmxValue);

  // Calculate allowed steps based on an angle DMX value.
  int stepsAllowed = stepsPerRevolution / (map(DMXSerial.read(angle), 0, 255, 2, 1));

  // Map DMX value (0–255) to a target step count (0 to stepsAllowed)
  long targetSteps = map(dmxValue, 0, 255, 0, stepsAllowed);

  // Update stepsPerUpdate based on DMX speed value.
  constantSpeed = map(DMXSerial.read(speed), 0, 255, 50, 500);

  // Only update if the stepInterval has elapsed.
  unsigned long now = micros();
  if (now - lastStepUpdate >= stepInterval) {
    lastStepUpdate = now;
    
    long diff = targetSteps - stepperMotor.currentPosition();
    
    if (diff > 0) {
      int stepsToTake = (diff > stepsPerUpdate) ? stepsPerUpdate : diff;
      stepperMotor.enableOutputs();
      stepperMotor.setSpeed(constantSpeed);
      
      // Countdown loop: decrement stepsToTake until zero.
      while (stepsToTake > 0) {
        stepperMotor.runSpeed();
        stepsToTake--;
      }
      
    } else if (diff < 0) {
      int stepsToTake = (-diff > stepsPerUpdate) ? stepsPerUpdate : -diff;
      stepperMotor.enableOutputs();
      stepperMotor.setSpeed(-constantSpeed);
      
      // Countdown loop for reverse motion.
      while (stepsToTake > 0) {
        stepperMotor.runSpeed();
        stepsToTake--;
      }
      
    } else {
      // Target reached: stop motor and disable outputs.
      stepperMotor.setSpeed(0);
      stepperMotor.disableOutputs();
    }
  }
    
  /*
  // Debugging output:
  Serial.print("DMX Value: ");
  Serial.print(dmxValue);
  Serial.print(" | Target Steps: ");
  Serial.print(targetSteps);
  Serial.print(" | Current Position: ");
  Serial.println(stepperMotor.currentPosition());
  */
}
