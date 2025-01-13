#include <Wire.h>
#include <HX711.h>
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
/**********************************************************************
  程序名称/Program name     : TCA Temperature linearity parameter $c$ identification
  作者/Author              : Yunsong Zhang 
  日期/Date（YYYYMMDD）     : 2025/1/13
  程序目的/Purpose          :
  辨识TCA的温度参数$c$, 具体实验细节则是,固定TCA然后通过输入功率 并测量当前温度与力 ,并测量记录.
Identify the TCA temperature parameter $c $, the details of the experiment is, fixed TCA and then through the input power, 
and measure the current temperature and force, and measure the record.

  -----------------------------------------------------------------------
  -----------------------------------------------------------------------
***********************************************************************/


// 定义HX711模块的DT和SCK引脚
#define LOADCELL_DOUT_PIN 14  // 连接到 ESP32 的 GPIO 14
#define LOADCELL_SCK_PIN 15   // 连接到 ESP32 的 GPIO 15

// MLX90640 相关定义
#define TA_SHIFT 8 //Default shift for MLX90640 in open air
const byte MLX90640_address = 0x39; //Default 7-bit unshifted address of the MLX90640

// PWM 和 INA/INB/INC 引脚定义
#define PWM_Pin 2  // 只使用一个PWM引脚
#define INA_Pin 8  // 控制H桥的引脚
#define INB_Pin 9  // 控制H桥的引脚

HX711 scale;
float mlx90640To[768];
paramsMLX90640 mlx90640;

long startTime = millis();

float Vout;
float R = 16;  // 
float duty_cycle;
float power = 0.0;  // 初始功率
float maxpower = 2.0;  // 最大功率
float Vinput = 15.0;  // 输入电压

void setup() {
  Wire.begin();
  Wire.setClock(400000); //Increase I2C clock speed to 400kHz

  Serial.begin(115200); //Fast serial as possible

  // 初始化HX711
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(1885.f);  // 这个值需要根据你的传感器和校准来设置
  scale.tare();  // 自动归零

  // 初始化MLX90640
  while (!Serial); //Wait for user to open terminal

  if (isConnected() == false) {
    Serial.println("MLX90640 not detected at default I2C address. Please check wiring. Freezing.");
    while (1);
  }

  //Get device parameters - We only have to do this once
  int status;
  uint16_t eeMLX90640[832];
  status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
  if (status != 0)
    Serial.println("Failed to load system parameters");

  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0)
    Serial.println("Parameter extraction failed");

  MLX90640_SetRefreshRate(MLX90640_address, 0x04); //Set rate to 8Hz

  // 初始化H桥控制引脚
  pinMode(INA_Pin, OUTPUT);
  pinMode(INB_Pin, OUTPUT);
  digitalWrite(INA_Pin, HIGH);  // 设置H桥方向
  digitalWrite(INB_Pin, LOW);
}

void loop() {
  // 检查串口输入
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == '1') {
      power += 0.1;  // 功率增加0.1W
      if (power > maxpower) {
        power = maxpower;  // 限制最大功率
      }
      Serial.print("Power increased to: ");
      Serial.print(power);
      Serial.println(" W");
    }
  }

  // 计算输出电压
  Vout = sqrt(power * R);
  duty_cycle = Vout / Vinput;
  analogWrite(PWM_Pin, (int)(duty_cycle * 255.0));

  // 读取MLX90640温度数据
  uint16_t mlx90640Frame[834];
  int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);

  float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
  float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

  float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
  float emissivity = 0.95;

  MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);

  // 过滤掉 NaN 值
  int validCount = 0;
  for (int i = 0; i < 768; i++) {
    if (!isnan(mlx90640To[i])) {
      mlx90640To[validCount++] = mlx90640To[i];
    }
  }

  // 对有效数据进行排序
  quickSort(mlx90640To, 0, validCount - 1);

  float hightemp = 0;
  float maxtem = 0;
  int k = 20;
  while (mlx90640To[0] - mlx90640To[k - 1] > 5) {
    k = k - 1;
  }
  for (int q = 1; q < k + 1; q++) {
    hightemp = mlx90640To[q] + hightemp;
  }
  maxtem = mlx90640To[1];
  hightemp = hightemp / k;

  // 读取力传感器数据
  float reading_force = 0;
  if (scale.is_ready()) {
    reading_force = scale.get_units(1);  // 读取力传感器数据
  }

  // 获取当前时间
  long stopTime = millis();
  double elapsedTime = (stopTime - startTime) / 1000.0;

  // 打印时间、功率、力和温度
  Serial.print("Time: ");
  Serial.print(elapsedTime);
  Serial.print(" s, Power: ");
  Serial.print(power);
  Serial.print(" W, Force: ");
  Serial.print(reading_force / 100.0);  // 单位是N
  Serial.print(" kg, Temperature: ");
  Serial.print(hightemp, 3);
  Serial.println(" ℃");

  delay(100);  // 适当延迟以控制输出频率
}

boolean isConnected() {
  Wire.beginTransmission((uint8_t)MLX90640_address);
  if (Wire.endTransmission() != 0)
    return (false); //Sensor did not ACK
  return (true);
}

// 快排函数
void quickSort(float arr[], int start, int length) {
  if (start < length) {
    int i = start, j = length;
    float x = arr[start];
    while (i < j) {
      while (i < j && arr[j] <= x) // 从右向左找第一个大于x的数
        j--;
      if (i < j)
        arr[i++] = arr[j];
      while (i < j && arr[i] > x) // 从左向右找第一个小于等于x的数
        i++;
      if (i < j)
        arr[j--] = arr[i];
    }
    arr[i] = x;
    quickSort(arr, start, i - 1); // 递归调用
    quickSort(arr, i + 1, length);
  }
}