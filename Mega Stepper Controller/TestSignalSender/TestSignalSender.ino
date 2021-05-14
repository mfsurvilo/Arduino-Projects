//This uses a seperate arduino, not the one attached to switchy or MegaValve. 
// It sends a 5V signal to test the mega stepper motor sheild. It emulates the signal that would be sent by switchy as it goes through the motions. 

long randNumber;

void setup() {
Serial.begin(9600);
pinMode(2,OUTPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
}

void loop() {
 randNumber = random(1,5);
 Serial.println(randNumber);
 
 if (randNumber == 1){
 digitalWrite (2,HIGH);
 delay(2000);
 digitalWrite (2,LOW);
 }

if (randNumber == 2){
 digitalWrite (3,HIGH);
 delay(2000);
 digitalWrite (3,LOW);
 }

 if (randNumber == 3){
 digitalWrite (4,HIGH);
 delay(2000);
 digitalWrite (4,LOW);
 }

 if (randNumber == 4){
 digitalWrite (5,HIGH);
 delay(2000);
 digitalWrite (5,LOW);
 }

 
}
