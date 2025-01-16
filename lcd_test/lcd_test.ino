#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the I2C LCD (address may be 0x27 or 0x3F, modify according to the actual address)
LiquidCrystal_I2C lcd(0x3C, 16, 2);  // 16 represents 16 columns, 2 represents 2 rows

void setup() {
  // Initialize the LCD
  lcd.init();  // Use init() for initialization

  // Display test text
  lcd.setCursor(0, 0);  // Set the cursor to the first row, first character position
  lcd.print("Hello, World!");
  lcd.setCursor(0, 1);  // Set the cursor to the second row, first character position
  lcd.print("I2C LCD Test");
}

void loop() {
  // Additional test logic or keeping the LCD content can be added here
  delay(1000);  // Keep the content displayed
}
