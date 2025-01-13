/**********************************************************************
  程序名称/Program name     : TCA Heat capacity parameter $C_th$ identification
  作者/Author              : Yunsong Zhang 
  日期/Date（YYYYMMDD）     : 2025/1/13
  程序目的/Purpose          :
  辨识TCA的热容量C_th, 具体实验细节则是,测量散热过程中位移的变化,进而得到时间常数.
The heat capacity Cth of the TCA is identified, and the details of the experiment are to measure the change of displacement during 
the heat dissipation, and then obtain the time constant.

  -----------------------------------------------------------------------
  -----------------------------------------------------------------------
***********************************************************************/

#include <Arduino_JSON.h>
#define INA_Pin1 8
#define INB_Pin1 9
#define INA_Pin2 10
#define INB_Pin2 11
const int pin = A0;
double lon;
unsigned long time;
int i = 0;

// 功率控制相关变量
float power = 0.0;  // 初始功率
float maxpower = 1.5;  // 最大功率（1.5W）
float Vinput = 15.0;  // 输入电压
float R = 16.0;  // 电阻
float Vout;
float duty_cycle;

void setup() {
  // 初始化串口通信
  Serial.begin(9600);

  // 初始化H桥控制引脚
  pinMode(INA_Pin1, OUTPUT);
  digitalWrite(INA_Pin1, HIGH);
  pinMode(INB_Pin1, OUTPUT);
  digitalWrite(INB_Pin1, LOW);
  pinMode(INA_Pin2, OUTPUT);
  digitalWrite(INA_Pin2, HIGH);
  pinMode(INB_Pin2, OUTPUT);
  digitalWrite(INB_Pin2, LOW);

  // 设置模拟参考电压
  analogReference(DEFAULT);
}

void loop() {
  // 检查串口输入
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == '1') {
      power = maxpower;  // 设置功率为1.5W
      Serial.print("Power increased to: ");
      Serial.print(power);
      Serial.println(" W");
    } else if (input == '0') {
      power = 0.0;  // 停止功率输入
      Serial.println("Power stopped");
    }
  }

  // 计算输出电压和占空比
  Vout = sqrt(power * R);  // 根据功率计算输出电压
  duty_cycle = Vout / Vinput;  // 计算占空比
  analogWrite(INA_Pin1, (int)(duty_cycle * 255.0));  // 设置PWM输出

  // 读取位移和时间
  lon = 270.0 - 32.0 * analogRead(pin) * 5 / 1024;  // 计算位移（单位：mm）
  time = millis();  // 获取当前时间（单位：ms）

  // 输出时间和位移
  Serial.print(time / 1000.0);  // 输出时间（单位：秒）
  Serial.print(" , ");
  Serial.println(lon);  // 输出位移（单位：mm）

  delay(100);  // 适当延迟以控制输出频率
}