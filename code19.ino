#include <IRremote.h>//包含红外库
#include<Servo.h>
Servo myservo;

int pos = 0;//舵机初始角度
int direc = 1;//决定舵机左右转动
int RECV_PIN = 11;//端口声明
int LED1 = 2;


long on1  = 0x00FF6897;//编码示例,与发送匹配用
long off1 = 0x00ff30CF;
long on2 = 0x00FF9867;
long off2 = 0x00FF18E7;

IRrecv irrecv(RECV_PIN);
decode_results results;//结构声明
// Dumps out the decode_results structure.
// Call this after IRrecv::decode()
// void * to work around compiler issue
//void dump(void *v) {
//  decode_results *results = (decode_results *)v
void dump(decode_results *results)
{
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN)
  {
    Serial.println("Could not decode message");
  }
  else
  {
    if (results->decode_type == NEC)
    {
      Serial.print("Decoded NEC: ");
    }
    else if (results->decode_type == SONY)
    {
      Serial.print("Decoded SONY: ");
    }
    else if (results->decode_type == RC5)
    {
      Serial.print("Decoded RC5: ");
    }
    else if (results->decode_type == RC6)
    {
      Serial.print("Decoded RC6: ");
    }
    Serial.print(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.println(" bits)");
  }
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 0; i < count; i++)
  {
    if ((i % 2) == 1)
    {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    }
    else
    {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
}

void setup()
{
  
  pinMode(RECV_PIN, INPUT);   //端口模式，输入
  pinMode(LED1, OUTPUT);//端口模式，输出

myservo.attach(10);
  pinMode(13, OUTPUT);////端口模式，输出
  Serial.begin(9600);	//波特率9600
  irrecv.enableIRIn(); // Start the receiver
  myservo.write(0);
  delay (200);
  

}

int on = 0;
unsigned long last = millis();

void loop()
{
 if (results.value == on2 )
 {//实现电风扇左右摇摆的效果
 if (pos<180&&(direc%2==1)){
             myservo.write(++pos);//   舵机旋转角度为pos
             delay(50);}
             if (pos ==180) direc++;
             if (pos >0&&(direc%2==0))
             myservo.write(--pos);
             delay(50);
             if (pos == 0) direc ++;
 }
  if (irrecv.decode(&results)) //调用库函数：解码
  {
    // If it's been at least 1/4 second since the last
    // IR received, toggle the relay
    if (millis() - last > 250)
    {
      on = !on;
      digitalWrite(13, on ? HIGH : LOW);
      dump(&results);
    }
    if (results.value == on1 )
      digitalWrite(LED1, HIGH);
      
     
    if (results.value == off1 )
      digitalWrite(LED1, LOW);
   
    last = millis();
    irrecv.resume(); // Receive the next value
  }
}
