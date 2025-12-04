#include <Wire.h>
#include <LiquidCrystal.h>

// Change to your LCD address (0x27 or 0x3F are common)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte barLeft[8] = {
  B10000, B10000, B11000, B11100,
  B11110, B11111, B11111, B11111
};

byte barCenter[8] = {
  B00100, B00100, B01110, B11111,
  B01110, B00100, B00100, B00100
};

byte barRight[8] = {
  B00001, B00001, B00011, B00111,
  B01111, B11111, B11111, B11111
};

// MPU6500 Registers
#define MPU6500_ADDR 0x68
#define WHO_AM_I     0x75
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43

void setup() {
  Wire.begin();
  Serial.begin(115200);

  lcd.clear();
  lcd.print("MPU6500 Init...");

  // Verify WHO_AM_I response
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(WHO_AM_I);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6500_ADDR, 1);

  byte whoami = Wire.read();

  lcd.clear();
  lcd.print("WHO_AM_I: ");
  lcd.print(whoami, HEX);

  Serial.print("WHO_AM_I: 0x");
  Serial.println(whoami, HEX);

  if (whoami != 0x70) {
    lcd.setCursor(0,1);
    lcd.print("NOT MPU6500!");
    while (1);   // stop
  }

  delay(1000);

  // Wake up chip
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);        // exit sleep mode
  Wire.endTransmission();
}

int16_t read16(uint8_t reg) {
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6500_ADDR, 2);
  return (Wire.read() << 8) | Wire.read();
}

void loop() {
  int16_t ax = read16(ACCEL_XOUT_H);
  int16_t ay = read16(ACCEL_XOUT_H + 2);
  int16_t az = read16(ACCEL_XOUT_H + 4);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AX:\n");
  lcd.print(ax);
  lcd.print(" AY:");
  lcd.print(ay);

  lcd.setCursor(0, 1);
  lcd.print("AZ:");
  lcd.print(az);

  delay(150);
}
