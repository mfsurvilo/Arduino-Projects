/*/////////////////////////////////////////////////////////////////////////////////////

Written by Matt Survilo.
For questions email msurvilo@gmail.com

PURPOSE - Code recieves a signal SWITCHY and sends PWM signals to a motor controller
for a large Stepper motor.
///////////////////////////////////////////////////////////////////////////////////

UPDATES LOG - If you update this file in anyway are save it for use in the GBRL shared drive 
please comment below the version you have created and what modifications you made to the code. 


////////////////////////////////////////////////////////////////////////////////////
UDFs - Functions and their purpose.

Home_ISR()     - Reads home switch signal. 
PrintStatus()  - Prints out status of valve input pins in serial monitor.
                 Should be used only when troubleshooting as it can slow down the time
                 between steps. 
Step()         - Moves motor a specific number of steps and a direction.
SpeedSet()     - Changes the delay between main pulses.
SlowStep()     - Does a slow single step used to find home
Home()         - Finds home position. 

////////////////////////////////////////////////////////////////////////////////////*/

////////////////////////////////
////  Declaration of Pins   ////
////////////////////////////////



const int dirPin = 3;     // Direction signal. (low is CW  high is CCW)
const int pulPin = 5;     // Pulse signal.
const int enPin = 7;     // Motor enable signal.

const int buttonPin = 2;  // Homing button
const int faultPin = 4;   // Motor fault signal output.
// These two need to be an interrupt pins. Check on the board.

const int V1Pin = A0;      // Input pins from switching valve controller.
const int V2Pin = A1;
const int V3Pin = A2;
const int V4Pin = A3;
const int LEDPin = 13;  // Change back to 12

////////////////////////////////////
////  Variable Initialization   ////
////////////////////////////////////
int valve;                // Current valve.
int destvalve;            // Destination valve.

volatile int buttonState = 0;
const int ppr = 20000;      // Pulse per revolution
float PDelay;         // Initial pulse delay
int RPM = 20;

String CW = "CW";       
String CCW = "CCW";

////////////////////////////////////
////  Matrix   Initialization   ////
////////////////////////////////////


int StepMatrix[4][4] = {  // Row numbers are the current valve position. 
  {0,     ppr/4,  ppr/2, ppr/4},       // Column numbers are the desination valve.
  {ppr/4,     0,  ppr/4, ppr/2},       // Values are the number of Steps to get to the new location. 
  {ppr/2, ppr/4,      0, ppr/4},       // Destination valve 1 has a high value because motor doesn't move a specific distance,
  {ppr/4, ppr/2,  ppr/4,     0}        // it moves till home is found. It rehomes everytime it goes to valve 1
 };

String dirMatrix[4][4] = {  // Row numbers are the current valve position. 
  {CW, CCW, CCW, CW},     // Column numbers are the desination valve.
  {CW, CW,  CCW, CW},     // Phrases are what direction motor needs to turn. 
  {CW, CW,  CW, CCW},
  {CCW, CW, CW, CW}
 };

////////////////////////////
////        Setup       ////
////////////////////////////

void setup() {

 Serial.begin(9600);
Serial.println("starting");
  pinMode(pulPin,OUTPUT); 
  digitalWrite(pulPin,LOW);
  pinMode(dirPin,OUTPUT);
  digitalWrite(dirPin,HIGH);
pinMode(enPin,OUTPUT);
  pinMode(buttonPin,INPUT);
  pinMode(faultPin,INPUT);
  pinMode(V1Pin,INPUT);         // Valve 1 Signal
  pinMode(V2Pin,INPUT);         // Valve 2 Signal
  pinMode(V3Pin,INPUT);         // Valve 3 Signal
  pinMode(V4Pin,INPUT);         // Valve 4 Signal
Serial.println("pinmodes set");
  // For info on how the below code works lookup Interrupts.
  attachInterrupt(digitalPinToInterrupt(buttonPin),Home_ISR,CHANGE); 

 digitalWrite(enPin,LOW);
PDelay = 200;
  Home();   
 digitalWrite(enPin,HIGH); 
  valve = 1;
  Serial.println("Found Home");
  
  //PDelay = SpeedSet(RPM,ppr);
  PDelay = 30;
  //Serial.print("PDelay at full speed is");
  //Serial.println(PDelay);
 // PrintStatus();

}

///////////////////////////
////        Loop       ////
///////////////////////////

void loop() { 
  delay(2000);
//destvalve = random(1,5);

  if (digitalRead(V1Pin) == LOW && digitalRead(V2Pin) == LOW && digitalRead(V3Pin) == LOW) {
    destvalve = 1;
  }
  if (digitalRead(V1Pin) == LOW && digitalRead(V2Pin) == LOW && digitalRead(V3Pin) == HIGH) {
    destvalve = 2;
  }
  if (digitalRead(V1Pin) == LOW && digitalRead(V2Pin) == HIGH && digitalRead(V3Pin) == LOW) {
    destvalve = 3;
  }
  if (digitalRead(V1Pin) == HIGH && digitalRead(V2Pin) == HIGH && digitalRead(V3Pin) == LOW) {
    destvalve = 4;
  }
  PrintStatus();


  if (valve != destvalve) {
    
     Step( StepMatrix [valve-1] [destvalve-1] , dirMatrix [valve-1] [destvalve-1] ,PDelay);
     valve = destvalve;
  }

}
