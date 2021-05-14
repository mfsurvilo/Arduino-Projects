
const int dirPin = 3;     // Direction signal. (low is CW  high is CCW)
const int pulPin = 5;     // Pulse signal.
const int enPin = 16;     // Motor enable signal.

const int ppr = 400;  
   
void setup() {
  pinMode(pulPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  Serial.begin(9600);
  // put your setup code here, to run once:
   digitalWrite(dirPin,HIGH);
}

void loop() {
  int PDelay = 150;
  int steps = 1200;
        digitalWrite(dirPin,HIGH);
        delay(500);
for (int x = 0; x<steps; x++) {
  
     

      digitalWrite(pulPin,HIGH);
      delayMicroseconds(PDelay);
      digitalWrite(pulPin,LOW);
      delayMicroseconds(PDelay);
}

delay(500);
  digitalWrite(dirPin,LOW);
for (int x = 0; x<steps; x++) {


      digitalWrite(pulPin,HIGH);
      delayMicroseconds(PDelay);
      digitalWrite(pulPin,LOW);
      delayMicroseconds(PDelay);
}


}
