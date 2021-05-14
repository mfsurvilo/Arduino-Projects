
int pot_in = A7;
int motor1 = 11;
int motor2 = 12;
unsigned long start1 = 0;
unsigned long start2 = 0;
unsigned long highstate1 = 1000;
unsigned long highstate2 = 1000;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

pinMode (pot_in,INPUT);
pinMode (motor1, OUTPUT);
pinMode (motor2, OUTPUT);
}

void loop() {  
unsigned long current1 = millis();

  if ((current1-start1)*1000 < highstate1) {
    digitalWrite (motor1,HIGH);
    delayMicroseconds(highstate1);
    digitalWrite (motor1, LOW);
    digitalWrite (motor2,HIGH);
    delayMicroseconds(highstate2);
    digitalWrite(motor2,LOW);
  }
  if ((current1-start1)*1000 < 20000-highstate1-highstate2 && (current1-start1)*1000 > highstate1) {
    digitalWrite(motor1,LOW);
  }

 
 if((current1-start1)*1000 > 20000) {
  
  unsigned long resistance = analogRead(pot_in);
  highstate1 = map(resistance,0,560,800,2000);
   highstate2 = 3000-highstate1;
  start1 = current1;
  }
}
