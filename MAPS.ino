#include <SoftwareSerial.h>

#include <Servo.h>
#define MAX 2400
#define MIN 555
#define FEED_DELAY 120 
#define FEED_PIN 3
#define FINE_STEPS 5
#define COARSE_STEPS 100
const uint16_t base_home=1477;
const uint16_t arm1_home=2000;
const uint16_t arm2_home=2000;
const uint16_t arm3_home=1477;

uint16_t motor_read[4]={base_home,arm1_home,arm2_home,arm3_home};
const uint16_t motor_offset[]={0,0,0,0};
const uint8_t step_delay=5;//delay between steps in ms
const uint8_t mpins[]={4,5,6,2};//base,a1,a2,a3
Servo motors[4];
Servo feeder;
SoftwareSerial BTSerial(11,12);

void manual_flush();
void feed_nudge();
void motor_write(uint8_t pin, uint16_t val);
void home_position();//get robot to home resting position slowly
void slow_go(uint8_t pin,uint16_t val);//get motor to certain value slowly


void feed_nudge()
{
  feeder.attach(FEED_PIN);
  delay(FEED_DELAY);
  feeder.detach();
}

void motor_write(uint8_t pin, uint16_t val)
{
  uint16_t value=val-motor_offset[pin];
  motors[pin].writeMicroseconds(val);
  motor_read[pin]=val;
}
void slow_go_toolpin1(uint16_t val)
{
  int i=motor_read[1];
  val=i-val;
  if (i<val)
  {
    for (;i<=val;i++)
    {
      motor_write(1,i);
      motor_write(3,--motor_read[3]);
      delay(step_delay);
    }
  }
  else
  {
    for (;i>=val;i--)
    {
      motor_write(1,i);
      motor_write(3,++motor_read[3]);
      delay(step_delay);
    }
  }
}

void slow_go_toolpin2(uint16_t val)
{
  int i=motor_read[2];
  val=i+val;
  if (i<val)
  {

    for (;i<=val;i++)
    {
      motor_write(2,i);
      motor_write(3,++motor_read[3]);
      delay(step_delay);
    }
  }
  else
  {
    for (;i>=val;i--)
    {
      motor_write(2,i);
      motor_write(3,--motor_read[3]);
      delay(step_delay);
    }
  }
}

void slow_go(uint8_t pin,uint16_t val)//add multi motor support
{
  int i=motor_read[pin];
  
//  Serial.println(i);
  if (i<val)
  {
//    Serial.println("First");

    for (;i<=val;i++)
    {
      motor_write(pin,i);
//      Serial.println(i);
      delay(step_delay);
    }
  }
  else
  {
//    Serial.println("Second");
    for (;i>=val;i--)
    {
      motor_write(pin,i);
//      Serial.println(i);
      delay(step_delay);
    }
  }
}
void home_position()
{
  slow_go(0,base_home);
  slow_go(1,arm1_home);
  slow_go(2,arm2_home);
  slow_go(3,arm3_home);
}

void manual_flush()
{
  while(BTSerial.available())
    char a = BTSerial.read();
}

void setup() 
{
  Serial.begin(9600);
  BTSerial.begin(38400);
//  home_position();
  motor_write(0,base_home);
  motor_write(1,arm1_home);
  motor_write(2,arm2_home);
  motor_write(3,arm3_home);
  //////////////////////
  feeder.write(180);//start the feeder forward
  /////////////////////
  for (int i=0;i<4;i++)
  {
    motors[i].attach(mpins[i]);
  }
//  feeder.attach(FEED_PIN);
}

void loop() 
{
  if(BTSerial.available())
  {

    char a= BTSerial.read();
    switch(a)
    {
      case 'n':
        feed_nudge();
        break;
      case 'u':
        slow_go(2,motor_read[2]-FINE_STEPS);
        slow_go_toolpin2(-FINE_STEPS);
        break;
      case 'U':
        slow_go(2,motor_read[2]-COARSE_STEPS);
        slow_go_toolpin2(-COARSE_STEPS);
        break;
      case 'd':
        slow_go(2,motor_read[2]+FINE_STEPS);
        slow_go_toolpin2(FINE_STEPS);
        break;
      case 'D':
        slow_go(2,motor_read[2]+COARSE_STEPS);
        slow_go_toolpin2(+COARSE_STEPS);
        break;
      case 'f':
        slow_go(1,motor_read[1]-FINE_STEPS);
        slow_go_toolpin1(FINE_STEPS);
        break;
      case 'F':
        slow_go(1,motor_read[1]-COARSE_STEPS);
        slow_go_toolpin1(COARSE_STEPS);
        break;
      case 'b':
        slow_go(1,motor_read[1]+FINE_STEPS);
        slow_go_toolpin1(-FINE_STEPS);
        break;
      case 'B':
        slow_go(1,motor_read[1]+COARSE_STEPS);
        slow_go_toolpin1(-COARSE_STEPS);
        break;
      case 'l':
        slow_go(0,motor_read[0]+FINE_STEPS);
        break;
      case 'L':
        slow_go(0,motor_read[0]+COARSE_STEPS);
        break;
      case 'r':
        slow_go(0,motor_read[0]-FINE_STEPS);
        break;
      case 'R':
        slow_go(0,motor_read[0]-COARSE_STEPS);
        break;
      
      case 'h':
        home_position();
        break;
      case 'c':
        int pin=BTSerial.parseInt();
        int val=BTSerial.parseInt();
        slow_go(pin,val);
        break;
      default:
        manual_flush();
        break;
    }
  }
//  int off=-500;
//  slow_go(2,2200);
//  slow_go(1,1700);
//  slow_go(3,2200);
//  delay(5000);
//  home_position();
//  while(1);
//  slow_go(0,1000);
//  slow_go(2,1700);
  
  
}
