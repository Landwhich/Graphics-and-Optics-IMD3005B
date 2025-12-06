#include <Arduino.h>
#include <Wire.h>
#include <PinChangeInterrupt.h>
#include <LiquidCrystal.h>

// --- MPU6500 Registers ---
#define MPU6500_ADDR 0x68
#define WHO_AM_I     0x75
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43

unsigned long lastLCD = 0;
const uint16_t lcdInterval = 200;

const byte buttonPin = 6;
volatile bool triggered = false;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte bar[8] = {
  0b11111,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11111,
};

int16_t read16(uint8_t reg) {
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6500_ADDR, 2);
  return (Wire.read() << 8) | Wire.read();
}

// void handleInterrupt() {
//   triggered = (digitalRead(interruptPin) == LOW);
// }

void setup() {
  Wire.begin();
  Serial.begin(115200);
  delay(200);

  pinMode(buttonPin, INPUT);
  // attachPCINT(digitalPinToPCINT(6), handleInterrupt, CHANGE);

  lcd.begin(16,2);
  lcd.createChar(0, bar);

  // lcd.print("Hello world");

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

  unsigned long now = millis();

  triggered = digitalRead(buttonPin) == HIGH ? true : false;

  // lcd updates slowed to reduce jitter
  if (now - lastLCD >= lcdInterval) {
    lastLCD = now;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Velocity");
    lcd.setCursor(0,1);
    lcd.write(byte(0));
    lcd.write(byte(0));
    lcd.write(byte(0));
    if(triggered){
      lcd.write(byte(0));
      lcd.write(byte(0));
      lcd.write(byte(0));
      lcd.write(byte(0));
    }
  }

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
  Serial.print(gz); Serial.print(",");
  Serial.println(triggered ? 1 : 0);

  delay(5); // 200 Hz output
}
