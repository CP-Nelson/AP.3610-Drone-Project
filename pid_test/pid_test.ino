#include <Wire.h>
#include <MPU6050.h>
#include <PID_v1.h>

// Initialize MPU6050 object
MPU6050 mpu;

// Target values (set to a level and stationary state)
double setpointPitch = 0;
double setpointRoll = 0;
double setpointYaw = 0;

// MPU6050 attitude inputs
double inputPitch, inputRoll, inputYaw;

// PID output values
double outputPitch, outputRoll, outputYaw;

// PID controllers
PID pidPitch(&inputPitch, &outputPitch, &setpointPitch, 2.0, 5.0, 1.0, DIRECT);
PID pidRoll(&inputRoll, &outputRoll, &setpointRoll, 2.0, 5.0, 1.0, DIRECT);
PID pidYaw(&inputYaw, &outputYaw, &setpointYaw, 2.0, 5.0, 1.0, DIRECT);

// Motor pin definitions
const int motorLeftTop = 6;  // Top-left
const int motorRightTop = 9; // Top-right
const int motorLeftBottom = 5;  // Bottom-left
const int motorRightBottom = 3; // Bottom-right

// Base motor speed (ensures the drone hovers in a level state)
int baseSpeed = 150;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1); // Halt the program
  }
  
  // Initialize PID controllers
  pidPitch.SetMode(AUTOMATIC);
  pidRoll.SetMode(AUTOMATIC);
  pidYaw.SetMode(AUTOMATIC);

  // Set output limits
  pidPitch.SetOutputLimits(-50, 50); // Adjust the range as needed
  pidRoll.SetOutputLimits(-50, 50);
  pidYaw.SetOutputLimits(-50, 50);
  
  // Configure motor pins
  pinMode(motorLeftTop, OUTPUT);
  pinMode(motorRightTop, OUTPUT);
  pinMode(motorLeftBottom, OUTPUT);
  pinMode(motorRightBottom, OUTPUT);
}

void loop() {
  // Retrieve angular velocity and acceleration data from MPU6050
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Calculate attitude angles
  inputPitch = atan2(ay, az) * 180 / PI;
  inputRoll = atan2(-ax, az) * 180 / PI;
  inputYaw = gz / 131.0; // Simplified yaw calculation using gyroscope data

  // Update PID controllers
  pidPitch.Compute();
  pidRoll.Compute();
  pidYaw.Compute();

  // Adjust motor speeds based on PID output
  int motorLT = baseSpeed + outputPitch + outputRoll - outputYaw; // Top-left
  int motorRT = baseSpeed + outputPitch - outputRoll + outputYaw; // Top-right
  int motorLB = baseSpeed - outputPitch + outputRoll + outputYaw; // Bottom-left
  int motorRB = baseSpeed - outputPitch - outputRoll - outputYaw; // Bottom-right

  // Limit motor speed range (0-255)
  motorLT = constrain(motorLT, 0, 255);
  motorRT = constrain(motorRT, 0, 255);
  motorLB = constrain(motorLB, 0, 255);
  motorRB = constrain(motorRB, 0, 255);

  // Output to motors
  analogWrite(motorLeftTop, motorLT);
  analogWrite(motorRightTop, motorRT);
  analogWrite(motorLeftBottom, motorLB);
  analogWrite(motorRightBottom, motorRB);

  // Output debug information
  Serial.print("Pitch: ");
  Serial.print(inputPitch);
  Serial.print(" | Roll: ");
  Serial.print(inputRoll);
  Serial.print(" | Yaw: ");
  Serial.print(inputYaw);
  Serial.print(" | Motors: ");
  Serial.print(motorLT);
  Serial.print(" ");
  Serial.print(motorRT);
  Serial.print(" ");
  Serial.print(motorLB);
  Serial.print(" ");
  Serial.println(motorRB);

  delay(10); // Control loop frequency
}
