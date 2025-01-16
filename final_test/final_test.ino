#include <Wire.h>
#include <MPU6050.h>
#include <PID_v1.h>

// 初始化MPU6050对象
MPU6050 mpu;

// 目标值（设为水平静止状态）
double setpointPitch = 0;
double setpointRoll = 0;
double setpointYaw = 0;

// MPU6050姿态输入
double inputPitch, inputRoll, inputYaw;

// PID输出值
double outputPitch, outputRoll, outputYaw;

// PID控制器
PID pidPitch(&inputPitch, &outputPitch, &setpointPitch, 2.0, 5.0, 1.0, DIRECT);
PID pidRoll(&inputRoll, &outputRoll, &setpointRoll, 2.0, 5.0, 1.0, DIRECT);
PID pidYaw(&inputYaw, &outputYaw, &setpointYaw, 2.0, 5.0, 1.0, DIRECT);

// 电机引脚定义
const int motorLeftTop = 6;  // 左上
const int motorRightTop = 9; // 右上
const int motorLeftBottom = 5;  // 左下
const int motorRightBottom = 3; // 右下

// 电机基础速度（保证无人机在水平状态下悬停）
int baseSpeed = 150;

void setup() {
  // 初始化串口
  Serial.begin(115200);
  
  // 初始化MPU6050
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1); // 停止程序
  }
  
  // 初始化PID控制器
  pidPitch.SetMode(AUTOMATIC);
  pidRoll.SetMode(AUTOMATIC);
  pidYaw.SetMode(AUTOMATIC);

  // 设置输出范围
  pidPitch.SetOutputLimits(-50, 50); // 调整范围，可根据实际需求修改
  pidRoll.SetOutputLimits(-50, 50);
  pidYaw.SetOutputLimits(-50, 50);
  
  // 配置电机引脚
  pinMode(motorLeftTop, OUTPUT);
  pinMode(motorRightTop, OUTPUT);
  pinMode(motorLeftBottom, OUTPUT);
  pinMode(motorRightBottom, OUTPUT);
}

void loop() {
  // 从MPU6050获取角速度和加速度数据
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // 姿态角计算
  inputPitch = atan2(ay, az) * 180 / PI;
  inputRoll = atan2(-ax, az) * 180 / PI;
  inputYaw = gz / 131.0; // 简化Yaw计算，使用陀螺仪数据

  // 更新PID控制器
  pidPitch.Compute();
  pidRoll.Compute();
  pidYaw.Compute();

  // 根据PID输出调整电机速度
  int motorLT = baseSpeed + outputPitch + outputRoll - outputYaw; // 左上
  int motorRT = baseSpeed + outputPitch - outputRoll + outputYaw; // 右上
  int motorLB = baseSpeed - outputPitch + outputRoll + outputYaw; // 左下
  int motorRB = baseSpeed - outputPitch - outputRoll - outputYaw; // 右下

  // 限制电机速度范围（0-255）
  motorLT = constrain(motorLT, 0, 255);
  motorRT = constrain(motorRT, 0, 255);
  motorLB = constrain(motorLB, 0, 255);
  motorRB = constrain(motorRB, 0, 255);

  // 输出到电机
  analogWrite(motorLeftTop, motorLT);
  analogWrite(motorRightTop, motorRT);
  analogWrite(motorLeftBottom, motorLB);
  analogWrite(motorRightBottom, motorRB);

  // 输出调试信息
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

  delay(10); // 控制循环频率
}
