const int potPins[] = {A0, A1, A2, A3};  // 电位器连接的模拟输入引脚数组
const int motorPins[] = {6, 7, 8, 9};    // 电机驱动连接的 PWM 输出引脚数组
int potValues[4];                        // 存储每个电位器的读入值
int motorSpeeds[4];                      // 存储每个电位器映射后的 PWM 值

void setup() {
  // 设置电机引脚为输出模式
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  Serial.begin(9600); // 初始化串口通信
}

void loop() {
  for (int i = 0; i < 4; i++) {
    // 读取电位器的模拟值（0 - 1023）
    potValues[i] = analogRead(potPins[i]);

    // 将电位器的值映射到 PWM 范围（0 - 255）
    motorSpeeds[i] = map(potValues[i], 0, 1023, 0, 255);

    // 输出 PWM 信号到对应电机驱动引脚
    analogWrite(motorPins[i], motorSpeeds[i]);
  }

  // 在串口监视器上显示每个电位器的值和对应的 PWM 值
  Serial.println("Potentiometer Values and Motor Speeds:");
  for (int i = 0; i < 4; i++) {
    Serial.print("A");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(potValues[i]);
    Serial.print(" -> PWM: ");
    Serial.println(motorSpeeds[i]);
  }
  
  delay(500); // 延时以便观察数值变化
}
