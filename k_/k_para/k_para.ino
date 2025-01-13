#include <SoftwareSerial.h>
#include <HX711.h>
/**********************************************************************
  程序名称/Program name     : TCA Stiffness parameter $k$ identification
  作者/Author              : Yunsong Zhang 
  日期/Date（YYYYMMDD）     : 2025/1/13
  程序目的/Purpose          :
  辨识TCA的刚度参数$k$, 具体实验细节则是,通过控制电机来控制TCA的长度测量对应长度的力量.然后记录当前时间力以及位移
The stiffness parameter of the TCA is identified, and the specific experimental details are that the length of the TCA 
is controlled by controlling the motor, and the force corresponding to the length is measured. 
Then record the current time force and displacement
  -----------------------------------------------------------------------
  -----------------------------------------------------------------------
***********************************************************************/

// 定义连接到OL440驱动器的Arduino引脚
const int stepPin = 5; 
const int dirPin = 6;  
const int ENpin = 7;  
SoftwareSerial mySerial(10, 11); // RX, TX

// 定义HX711力传感器的引脚
#define LOADCELL_DOUT_PIN 14  // 连接到 ESP32 的 GPIO 18
#define LOADCELL_SCK_PIN 15   // 连接到 ESP32 的 GPIO 19

HX711 scale;

float time_end;
float time_start;
float reading_force = 0;

// 自定义初始长度（单位：毫米）
float initial_length = 100.0; // 初始长度，可以根据需要更改

void setup() {
  // 设置步进电机引脚为输出模式
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(ENpin, OUTPUT);
  digitalWrite(ENpin, HIGH);

  // 初始化串口通信
  Serial.begin(9600);

  // 初始化力传感器
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(1885.f);  // 这个值需要根据你的传感器和校准来设置
  scale.tare();  // 自动归零
}

void loop() {
  if (Serial.available() > 0) {
    int input = Serial.read(); // 读取串口输入

    // 检查输入是否为'1'或'2'，控制步进电机
    if (input == '1' || input == '2') {
      // 设置电机转动方向，HIGH为一个方向，LOW为另一个方向
      digitalWrite(dirPin, input == '1' ? HIGH : LOW);

      time_start = millis();
      // 让电机旋转1000步
      for(int x = 0; x < 1000; x++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500); // 控制脉冲宽度，根据你的应用可能需要调整
        digitalWrite(stepPin, LOW);
        delayMicroseconds(500);
      }
      time_end = millis();

      // 读取力传感器数据
      if (scale.is_ready()) {
        reading_force = scale.get_units(1);  // 读取1次取平均值
      }

      // 计算实际长度（1000个脉冲对应10mm）
      float length = initial_length + (1000 * 10.0 / 1000.0); // 每次增加10mm

      // 打印时间、长度和力
      Serial.print((time_end - time_start) / 1000.0); // 时间（秒）
      Serial.print(" , ");
      Serial.print(length); // 长度（毫米）
      Serial.print(" , ");
      Serial.println(reading_force / 100.0); // 力（单位：N）

      // 更新初始长度
      initial_length = length;
    }
  }
}