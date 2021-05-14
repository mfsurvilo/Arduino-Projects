/* 
This is a sketch that uses the AdaFruit Motor shield to test the concept of the mega valve controller. 
*/
// Utilizes input pullup

#include <Wire.h>
#include <Adafruit_MotorShield.h>


// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 


// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #1 (M1 and M2)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 1);

int valve = 0;

void printstatus() {
int pin8 = digitalRead(8);
int pin9 = digitalRead(9);     
int pin10 = digitalRead(10);
int pin11 = digitalRead(11);
int pin12 = digitalRead(12);
Serial.print ("Pin 12 is ");
Serial.println(pin12);
Serial.print("Pin 11 is ");
Serial.println(pin11);
Serial.print("Pin 10 is ");
Serial.println(pin10);
Serial.print("Pin 9 is ");
Serial.println(pin9);
Serial.print("Pin 8 is ");
Serial.println(pin8);
delay(2);}

void setup() {
  
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Starting Program");

  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  pinMode(11,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
printstatus();
 
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(100);  // 100 rpm

  


// Find Home

  while (digitalRead(12) == HIGH) {
  Serial.println("InWhileLoop");
  printstatus();
  delay(10);
  myMotor->onestep(FORWARD, DOUBLE);
  valve = 1;

  }

 myMotor->setSpeed(500); // 500 rpm
  
}






void loop() {


  if (digitalRead(8) == LOW) {
// Move to Position 1
  
   if (valve == 1) {
   //Don't Move
   }
   if (valve == 2) {
    myMotor->step(50, BACKWARD, DOUBLE); 
   }
   if(valve == 3) {
    myMotor->step(100,BACKWARD,DOUBLE);
   }
   if(valve == 4) {
    myMotor->step(50,FORWARD, DOUBLE);
   }
   valve = 1;
   myMotor->release(); 
  } 


  if (digitalRead(9) == LOW) {
// Move to Position 2
  
   if (valve == 1) {
   myMotor->step(50, FORWARD, DOUBLE); 
   }
   if (valve == 2) {
    //Don't Move
   }
   if(valve == 3) {
    myMotor->step(50,BACKWARD,DOUBLE);
   }
   if(valve == 4) {
    myMotor->step(100,FORWARD, DOUBLE);
   }
   valve = 2;
   myMotor->release(); 
  } 


  if (digitalRead(10) == LOW) {
// Move to Position 3
  
   if (valve == 1) {
   myMotor->step(100, FORWARD, DOUBLE); 
   }
   if (valve == 2) {
    myMotor->step(50,FORWARD,DOUBLE);
   }
   if(valve == 3) {
    //Don't Move
   }
   if(valve == 4) {
    myMotor->step(50,BACKWARD, DOUBLE);
   }
   valve = 3;
   myMotor->release(); 
  } 

  if (digitalRead(11) == LOW) {
// Move to Position 4
   if (valve == 1) {
   myMotor->step(50, BACKWARD, DOUBLE); 
   }
   if (valve == 2) {
    myMotor->step(100,FORWARD,DOUBLE);
   }
   if(valve == 3) {
    myMotor->step(50, FORWARD, DOUBLE); 
   }
   if(valve == 4) {
    //Don't Move
   }
   valve = 4;
   myMotor->release();   
  } 
  
}
