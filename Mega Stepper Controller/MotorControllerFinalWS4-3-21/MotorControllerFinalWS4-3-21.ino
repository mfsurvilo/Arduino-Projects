/*/////////////////////////////////////////////////////////////////////////////////////

Written by Matt Survilo.
For questions email msurvilo@gmail.com

PURPOSE - Code recieves a signal SWITCHY and sends pulse signals to a motor controller
for a large stepper motor (HS4220).
///////////////////////////////////////////////////////////////////////////////////

UPDATES LOG - If you update this file in anyway are save it for use in the GBRL shared drive 
please comment below the version you have created and what modifications you made to the code. 

7-30-20  Matt Survilo- Seperated UDFs into seperate functions. Working on integrating switch function to account for drift and step loss.
The plan was to rehome every so often maybe every 100 cylcles or something like that. 

////////////////////////////////////////////////////////////////////////////////////
UDFs - Functions and their purpose.

Home_ISR()     - Reads home switch signal. 
Fault_ISR()    - Reads fault signal from motor controller.
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
const int enPin = 16;     // Motor enable signal.

const int buttonPin = 2;  // Homing button
const int faultPin = 4;   // Motor fault signal output.
// These two need to be an interrupt pins. Check on the board.

const int V1Pin = 8;      // Input pins from switching valve controller.
const int V2Pin = 9;
const int V3Pin = 10;
const int V4Pin = 11;
const int LEDPin = 12;

////////////////////////////////////
////  Variable Initialization   ////
////////////////////////////////////
int valve;                // Current valve.
int destvalve;            // Destination valve.
int count;                // Keeps track of full cycles without a rehoming calibration. 
const int rehomecount = 4 // The number of cylces before it uses the switch to find home. 

volatile int buttonState = 0;
const int ppr = 400;      // Pulse per revolution
int RPM = 60;                  // Full speed motor RPM

char CW = "CW";       
char CCW = "CCW";

////////////////////////////////////
////  Matrix   Initialization   ////
////////////////////////////////////


int StepMatrix[4][4] = {  // Row numbers are the current valve position. 
  {0,     ppr/4,  ppr/2, ppr/4},       // Column numbers are the desination valve.
  {ppr/4,     0,  ppr/4, ppr/2},       // Values are the number of Steps to get to the new location. 
  {ppr/2, ppr/4,      0, ppr/4},       // Destination valve 1 has a high value because motor doesn't move a specific distance,
  {ppr/4, ppr/2,  ppr/4,     0}        // it moves till home is found. It rehomes everytime it goes to valve 1
 };

char dirMatrix[4][4] = {  // Row numbers are the current valve position. 
  {CW, CCW, CCW, CW},     // Column numbers are the desination valve.
  {CW, CW,  CCW, CW},     // Phrases are what direction motor needs to turn. 
  {CW, CW,  CW, CCW},
  {CCW, CW, CW, CW}
 };

////////////////////////////
////        Setup       ////
////////////////////////////

void setup() {
 

  pinMode(pulPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  pinMode(buttonPin,INPUT);
  pinMode(faultPin,INPUT);
  pinMode(V1Pin,INPUT);         // Valve 1 Signal
  pinMode(V2Pin,INPUT);         // Valve 2 Signal
  pinMode(V3Pin,INPUT);         // Valve 3 Signal
  pinMode(V4Pin,INPUT);         // Valve 4 Signal

  // For info on how the below code works lookup Interrupts.
  attachInterrupt(digitalPinToInterrupt(buttonPin),Home_ISR,CHANGE); 
  attachInterrupt(digitalPinToInterrupt(faultPin),fault_ISR,CHANGE);  
  
  Serial.begin(9600);
  
  Home();
  valve = 1;
  Serial.println("Found Home");
}

///////////////////////////
////        Loop       ////
///////////////////////////

void loop() { 
  delay(1000);
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



  if (valve != destvalve) {                 // Check for new valve orders
     if (destvalve == 1) {                  // Is new position valve 1?
      count++ ;                             // Add to the count
      if (count > rehomecount) {            // If the count exceeds threshold using ReHoming procedure. 
        ReHome( StepMatrix [valve-1] [destvalve-1] , dirMatrix [valve-1] [destvalve-1], ppr);
        count = 0;
      }
      
      else {
          Step( StepMatrix [valve-1] [destvalve-1] , dirMatrix [valve-1] [destvalve-1], RPM );
                                            // Continue with business as usual.                                 
      } 
     }
     valve = destvalve;
  }
}
