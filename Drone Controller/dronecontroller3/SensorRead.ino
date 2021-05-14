#include "MyHeader.h"
float SensorRead(){
  
float sumx = 0;
float sumy = 0;
float sumz = 0;
float zerogbias = 5/2;



for (int i = 0; i<avgrun ; ++i){
  sumx = analogRead(xpin)+sumx;
  sumy = analogRead(ypin)+sumy;
  sumz = analogRead(zpin)+sumz;
  delay(2);
  }

float Voltsx = (sumx/avgrun)*analogRead(arefpin);
float Voltsy = (sumy/avgrun)*analogRead(arefpin);
float Voltsz = (sumz/avgrun)*analogRead(arefpin);

float Ax = Voltsx-zerogbias;
float Ay = Voltsy-zerogbias;
float Az = Voltsz-zerogbias;

float Roll = asin(Ay);
float Pitch = asin(Ax);

Serial.print("Roll =  ");
Serial.println(Roll);
Serial.print("Pitch =  ");
Serial.println(Pitch);
}
