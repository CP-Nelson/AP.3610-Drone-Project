#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 初始化 I2C LCD（地址可能是 0x27 或 0x3F，请根据实际地址更改）
LiquidCrystal_I2C lcd(0x3C, 16, 2);  // 16 表示 16 列，2 表示 2 行

void setup() {
  // 初始化 LCD
  lcd.init();  // 使用 init() 进行初始化

  // 显示测试文本
  lcd.setCursor(0, 0);  // 设置光标到第一行第一个字符位置
  lcd.print("Hello, World!");
  lcd.setCursor(0, 1);  // 设置光标到第二行第一个字符位置
  lcd.print("I2C LCD Test");
}

void loop() {
  // 可以在这里添加其他测试逻辑或使 LCD 保持显示内容
  delay(1000);  // 保持显示内容
}
