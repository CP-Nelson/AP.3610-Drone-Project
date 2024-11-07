const int potPin = A0;       // 电位器连接的模拟输入引脚
const int motorPin = 9;      // 电机驱动连接的 PWM 输出引脚
int potValue = 0;            // 存储电位器读入的值
int motorSpeed = 0;          // 存储映射后的 PWM 值

void setup() {
  pinMode(motorPin, OUTPUT); // 设置电机引脚为输出模式
  Serial.begin(9600);        // 初始化串口通信
}

void loop() {
  // 读取电位器的模拟值（0 - 1023）
  potValue = analogRead(potPin);
  
  // 将电位器的值映射到 PWM 范围（0 - 255）
  motorSpeed = map(potValue, 0, 1023, 0, 255);
  
  // 输出 PWM 信号到电机驱动引脚
  analogWrite(motorPin, motorSpeed);
  
  // 在串口监视器上显示电位器值和 PWM 值
  Serial.print("Potentiometer Value: ");
  Serial.print(potValue);
  Serial.print(" -> Motor PWM Value: ");
  Serial.println(motorSpeed);
  
  delay(500); // 延时以便观察数值变化
}
