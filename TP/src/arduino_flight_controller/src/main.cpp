#include <Arduino.h>
#include <Wire.h>

// --- MPU6500 Registers ---
#define MPU6500_ADDR 0x68
#define WHO_AM_I     0x75
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43

int16_t read16(uint8_t reg) {
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6500_ADDR, 2);
  return (Wire.read() << 8) | Wire.read();
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  delay(200);

  // Read WHO_AM_I
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(WHO_AM_I);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6500_ADDR, 1);
  uint8_t who = Wire.read();

  Serial.print("WHO_AM_I = 0x");
  Serial.println(who, HEX);

  if (who != 0x70) {
    Serial.println("ERROR: Not MPU6500");
    while (1);
  }

  // Wake
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);
  Wire.endTransmission();
}

void loop() {
  int16_t ax = read16(ACCEL_XOUT_H);
  int16_t ay = read16(ACCEL_XOUT_H + 2);
  int16_t az = read16(ACCEL_XOUT_H + 4);

  int16_t gx = read16(GYRO_XOUT_H);
  int16_t gy = read16(GYRO_XOUT_H + 2);
  int16_t gz = read16(GYRO_XOUT_H + 4);

  // Send compact readable line
  Serial.print(ax); Serial.print(",");
  Serial.print(ay); Serial.print(",");
  Serial.print(az); Serial.print(",");
  Serial.print(gx); Serial.print(",");
  Serial.print(gy); Serial.print(",");
  Serial.println(gz);

  delay(5); // 200 Hz output
}
