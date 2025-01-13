#include <Arduino_JSON.h>
/**********************************************************************
  程序名称/Program name     : TCA damping parameter $b$ identification
  作者/Author              : Yunsong Zhang 
  日期/Date（YYYYMMDD）     : 2025/1/13
  程序目的/Purpose          :
  辨识TCA的阻尼参数$b$, 具体实验细节则是,通过在TCA末端悬挂重物,然后拉伸一个偏差位移后,使TCA进行衰减震荡运动,并测量记录位移与时间
  The damping parameter $b $of the TCA is identified by suspending a weight at the end of the TCA and then stretching a deviation displacement 
  to cause the TCA to undergo damped oscillatory motion, and the displacement and time are measured.
  -----------------------------------------------------------------------
  -----------------------------------------------------------------------
***********************************************************************/


#define INA_Pin1 8
#define INB_Pin1 9
#define INA_Pin2 10
#define INB_Pin2 11
const int pin = A0;
double lon;
unsigned long time;
int i=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(INA_Pin1,OUTPUT);
  digitalWrite(INA_Pin1,HIGH);
  pinMode(INB_Pin1,OUTPUT);
  digitalWrite(INB_Pin1,LOW);
  pinMode(INA_Pin2,OUTPUT);
  digitalWrite(INA_Pin2,HIGH);
  pinMode(INB_Pin2,OUTPUT);
  digitalWrite(INB_Pin2,LOW);

    analogReference(DEFAULT); 

}
void loop() {
  // put your main code here, to run repeatedly:
  lon=270.0-32.0*analogRead(pin)*5/1024;//mm
  time=millis();//ms
    Serial.print(time/1000.0);
    Serial.print(" , ");
    Serial.println(lon);
}
