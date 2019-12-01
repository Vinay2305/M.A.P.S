#include <SoftwareSerial.h>


#include <MPU6050_tockn.h>
#include <Wire.h>


#define BT_PIN 12
#define F1 8
#define F2 7
#define F3 6
#define F4 4
#define VIB A3
#define SOFT_BUZZ_DELAY 100
#define HARD_BUZZ_DELAY 200
#define HOME_BUZZ_DELAY 350
#define DEBOUNCE_DELAY 160
#define THRESHOLD 360
#define ACC_THRESHOLD 2.9




MPU6050 mpu6050(Wire);

SoftwareSerial BTSerial(99,BT_PIN);

float gyrox,gyroy,gyroz,acc;
void soft_buzz();
void hard_buzz();
void home_buzz();
void get_data()
{
  mpu6050.update();
  gyrox=mpu6050.getGyroX();
  gyroy=mpu6050.getGyroY();
  gyroz=mpu6050.getGyroZ();
  acc=sqrt(sq(mpu6050.getAccX())+sq(mpu6050.getAccY())+sq(mpu6050.getAccZ()));
//  Serial.println(accz);
}

void soft_buzz()
{
  digitalWrite(VIB,HIGH);
  delay(SOFT_BUZZ_DELAY);
  digitalWrite(VIB,LOW);
}
void hard_buzz()
{
  digitalWrite(VIB,HIGH);
  delay(HARD_BUZZ_DELAY);
  digitalWrite(VIB,LOW);
}
void home_buzz()
{
  digitalWrite(VIB,HIGH);
  delay(HOME_BUZZ_DELAY);
  digitalWrite(VIB,LOW);
}
void setup() 
{
  pinMode(VIB, OUTPUT);
  pinMode(F1, INPUT_PULLUP);
  pinMode(F2, INPUT_PULLUP);
  pinMode(F3, INPUT_PULLUP);
  pinMode(F4, INPUT_PULLUP);
  
  Wire.begin();
  home_buzz();
  delay(100);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  home_buzz();
  BTSerial.begin(38400);
}

void loop() 
{
  get_data();//update


  if(digitalRead(F3)==LOW)
  {
    BTSerial.print("n");
    hard_buzz();
    while(digitalRead(F3)==LOW);   
  }
  if(digitalRead(F1)&&digitalRead(F2))
  {
    if(acc>ACC_THRESHOLD)
    {
      BTSerial.print("h");
      home_buzz();
    }
  }
        
  
  else if (abs(gyrox)>THRESHOLD || abs(gyroy)>THRESHOLD || abs(gyroz)>THRESHOLD)
  {
    if(abs(gyrox)>abs(gyroy) && abs(gyrox)>abs(gyroz))//Is gyrox the greatest?
    {
      if(gyrox>0)//Is gyrox positive?
      {
        if(digitalRead(F2)==LOW)//Fine
        {
          BTSerial.print("r");
          soft_buzz();
        }
        else if(digitalRead(F1)==LOW)//Low priority Coarse
        {
          BTSerial.print("R");
          hard_buzz();
        }
      }
      else//Not Positive
      {
        if(digitalRead(F2)==LOW)//Fine
        {
          BTSerial.print("l");
          soft_buzz();
        }
        else if(digitalRead(F1)==LOW)//Low priority Coarse
        {
          BTSerial.print("L");
          hard_buzz();
        }
      }
    }
    else if(abs(gyroy)>abs(gyrox) && abs(gyroy)>abs(gyroz))//Is gyroy the greatest?
    {
      if(gyroy>0)//Is gyroy positive?
      {
        if(digitalRead(F2)==LOW)//Fine
        {
          BTSerial.print("f");
          soft_buzz();
        }
        else if(digitalRead(F1)==LOW)//Low priority Coarse
        {
          BTSerial.print("F");
          hard_buzz();
        }
      }
      else//Not Positive
      {
        if(digitalRead(F2)==LOW)//Fine
        {
          BTSerial.print("b");
          soft_buzz();
        }
        else if(digitalRead(F1)==LOW)//Low priority Coarse
        {
          BTSerial.print("B");
          hard_buzz();
        }
      }
    }
    else
    {
      if(gyroz>0)//Is gyroy positive?
      {
        if(digitalRead(F2)==LOW)//Fine
        {
          BTSerial.print("u");
          soft_buzz();
        }
        else if(digitalRead(F1)==LOW)//Low priority Coarse
        {
          BTSerial.print("U");
          hard_buzz();
        }
      }
      else//Not Positive
      {
        if(digitalRead(F2)==LOW)//Fine
        {
          BTSerial.print("d");
          soft_buzz();
        }
        else if(digitalRead(F1)==LOW)//Low priority Coarse
        {
          BTSerial.print("D");
          hard_buzz();
        }
      }
    }
    delay(DEBOUNCE_DELAY);
  }
  
  
}
