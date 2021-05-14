#include <Servo.h>
#include "MyHeader.h"

Servo motor1;
Servo motor2;

int motorspeed[2];
void setup() {
Serial.begin(9600);
pinMode (pot_in,INPUT);

analogReference(EXTERNAL);

pinMode(A1,OUTPUT);
pinMode(A0,OUTPUT);

digitalWrite(A1,LOW);
digitalWrite(A0,HIGH);

motor1.attach(11);
motor2.attach(12);

//Initialize setup
motor1.writeMicroseconds(1000);
motor2.writeMicroseconds(1000);
delay(5000);
}

void loop() {  
throttle = map(analogRead(pot_in),0,1023,0,1);
SensorRead();

}
