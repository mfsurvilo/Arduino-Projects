/*
SparkFun Inventor’s Kit
Circuit 1B-Potentiometer

Changes how fast an LED connected to pin 13 blinks, based on a potentiometer connected to pin A0

This sketch was written by SparkFun Electronics, with lots of help from the Arduino community.
This code is completely free for any use.

View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40
Download code at: https://github.com/sparkfun/SIK-Guide-Code
*/

#include <LiquidCrystal.h>          //the liquid crystal libarry contains commands for printing to the display

LiquidCrystal lcd(12, 11, 10, 9, 8, 7); 
int potPosition ;
int potVal;

//this variable will hold a value based on the position of the potentiometer

void setup()
{  lcd.begin(16, 2);                 //tell the lcd library that we are using a display that is 16 characters wide and 2 characters high
  lcd.clear();                      //clear the display


  Serial.begin(9600);       //start a serial connection with the computer

  pinMode(13, OUTPUT);      //set pin 13 as an output that can be set to HIGH or LOW
}

void loop()
{
  //read the position of the pot
  potPosition = analogRead(A0); 
  float voltage = potPosition * (5.0 / 1023);   //set potPosition to a number between 0 and 1023 based on how far the knob is turned
  Serial.println(voltage);     //print the value of potPosition in the serial monitor on the computer

 
  digitalWrite(13, HIGH);           // Turn on the LED
   delay(potPosition);              // delay for as many miliseconds as potPosition (0-1023)

digitalWrite(13, LOW);            // Turn off the LED
  delay(potPosition);              // delay for as many miliseconds as potPosition (0-1023)

   lcd.setCursor(0,0);               //set the cursor to the 0,0 position (top left corner)
  lcd.print(voltage);       //print hello, world! starting at that position

}
