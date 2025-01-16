#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;

int16_t ax, ay, az;  // Raw accelerometer data
int16_t gx, gy, gz;  // Raw gyroscope data

// Offsets for accelerometer and gyroscope (at rest)
int16_t ax_offset, ay_offset, az_offset;
int16_t gx_offset, gy_offset, gz_offset;

// Actual values after subtracting offsets
float ax_g, ay_g, az_g; // Accelerometer in g
float gx_dps, gy_dps, gz_dps; // Gyroscope in °/s

void setup() {
  Wire.begin();
  Serial.begin(38400);

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (true);
  }

  Serial.println("MPU6050 connection successful");

  // Read the initial sensor values (at rest)
  mpu.getMotion6(&ax_offset, &ay_offset, &az_offset, &gx_offset, &gy_offset, &gz_offset);
  Serial.println("Default offsets set:");
  Serial.print("Accel Offsets (X/Y/Z): ");
  Serial.print(ax_offset); Serial.print("\t");
  Serial.print(ay_offset); Serial.print("\t");
  Serial.print(az_offset); Serial.println();
  Serial.print("Gyro Offsets (X/Y/Z): ");
  Serial.print(gx_offset); Serial.print("\t");
  Serial.print(gy_offset); Serial.print("\t");
  Serial.println(gz_offset);
  
  // Ensure sensor is stationary during calibration
  delay(2000);  // Delay for sensor to settle
}

void loop() {
  // Read raw accelerometer and gyroscope data
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Subtract the stored offsets from the new readings to get actual values
  ax_g = (ax - ax_offset) / 16384.0;
  ay_g = (ay - ay_offset) / 16384.0;
  az_g = (az - az_offset) / 16384.0;

  gx_dps = (gx - gx_offset) / 131.0;
  gy_dps = (gy - gy_offset) / 131.0;
  gz_dps = (gz - gz_offset) / 131.0;

  // Print the actual values
  Serial.print("Actual Acceleration (g):\t");
  Serial.print(ax_g); Serial.print("\t");
  Serial.print(ay_g); Serial.print("\t");
  Serial.print(az_g); Serial.println();

  Serial.print("Actual Angular Velocity (°/s):\t");
  Serial.print(gx_dps); Serial.print("\t");
  Serial.print(gy_dps); Serial.print("\t");
  Serial.print(gz_dps); Serial.println();

  delay(500); // Delay to make the output readable
}
