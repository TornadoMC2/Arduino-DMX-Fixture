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
float constantSpeed = 1000.0;          // Increased constant speed (steps per second)
float maxSpeedVal    = 1000.0;         // Increased maximum speed
const int stepsPerUpdate = 75;        // Maximum number of steps to take per update
unsigned long lastStepUpdate = 0;     // Time (in microseconds) of last step update
unsigned long stepInterval = 2000;    // Minimum time (in microseconds) between update blocks

void setup() {
  // Initialize DMXSerial in receiver mode and set channel 1 as max
  DMXSerial.init(DMXReceiver);
  //DMXSerial.maxChannel(1);

  //DMXSerial.write(1, 255);

  // Set maximum speed and acceleration (acceleration is not used in runSpeed mode)
  stepperMotor.setMaxSpeed(maxSpeedVal);
  // Using runSpeed() bypasses acceleration, so acceleration setting is not needed.
  
  // Enable outputs to drive the motor initially
  stepperMotor.enableOutputs();
  
  Serial.begin(250000);
  Serial.println("AccelStepper linear DMXStepper (fast) controller started.");

  // Configure switch pins as inputs with pull-ups
  for (int i = 0; i < POSITION_NUM; i++) {
    pinMode(SWITCH_PINS[i], INPUT_PULLUP);
  }
}

void loop() {
  // Build a 10-bit encoded state from the 10 switch pins.
  int encoded_state = 1;
  /*for (int i = 0; i < POSITION_NUM; i++) {
    if (digitalRead(SWITCH_PINS[i]) == ON)
      encoded_state |= 1 << (POSITION_NUM - i - 1);
  }*/

  /*if (Serial.available() > 0) {
    // Read characters until a newline is received.
    String numStr = Serial.readStringUntil('\n');
    numStr.trim();  // Remove any leading/trailing whitespace
    if (numStr.length() > 0) {
      int dmxValue = numStr.toInt();
      Serial.print("Read value: ");
      Serial.println(dmxValue);
    }
  }

  int dmxValue = Serial.parseInt();

  while (Serial.available() > 0) {
    Serial.read();
  }*/


  Serial.print("Current DMX Address: ");
  Serial.println(encoded_state);

  int angle = encoded_state;
  int speed = encoded_state+1;
  int value = encoded_state+2;

  // Read DMX value from the channel given by encoded_state.
  int dmxValue = DMXSerial.read(value);
  Serial.print("Read value: ");
  Serial.println(DMXSerial.read(value));


  // This allows chosing of an angle by dividing the stepsPerRevolution by the angle DMX value
  int stepsAllowed = stepsPerRevolution / (map(DMXSerial.read(angle), 0, 255, 2, 1));

  // Map DMX value (0–255) to a target step count (0 to stepsFor180)
  long targetSteps = map(dmxValue, 0, 255, 0, stepsAllowed);

  // Update the steps allowed per update to allow faster and slower speeds
  stepsPerUpdate = map(DMXSerial.read(speed), 0, 255, 1, 75)


  // Get current time in microseconds
  unsigned long now = micros();
  if (now - lastStepUpdate >= stepInterval) {
    lastStepUpdate = now;
    
    long diff = targetSteps - stepperMotor.currentPosition();
    
    if (diff > 0) {
      // Move forward up to stepsPerUpdate steps
      int stepsToTake = (diff > stepsPerUpdate) ? stepsPerUpdate : diff;
      // Ensure outputs are enabled before moving
      stepperMotor.enableOutputs();
      // Set the constant speed (positive for forward)
      stepperMotor.setSpeed(constantSpeed);
      for (int i = 0; i < stepsToTake; i++) {
        stepperMotor.runSpeed();
      }
    } else if (diff < 0) {
      int stepsToTake = (-diff > stepsPerUpdate) ? stepsPerUpdate : -diff;
      stepperMotor.enableOutputs();
      // Set speed negative for reverse movement
      stepperMotor.setSpeed(-constantSpeed);
      for (int i = 0; i < stepsToTake; i++) {
        stepperMotor.runSpeed();
      }
    } else {
      // Target reached: stop motor and disable outputs (pins go LOW)
      stepperMotor.setSpeed(0);
      stepperMotor.disableOutputs();
    }
  }

  // Optional debugging output:
  Serial.print("DMX Value: ");
  Serial.print(dmxValue);
  Serial.print(" | Target Steps: ");
  Serial.print(targetSteps);
  Serial.print(" | Current Position: ");
  Serial.println(stepperMotor.currentPosition());

  //delay(10); // Brief delay for serial stability; adjust if needed.
}
