// Checks all combinations of relay switching.
// Was used to find out what relay combinations

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
digitalWrite(2,HIGH);
delay(1500);

digitalWrite(3,HIGH);
delay(1500);

digitalWrite(4,HIGH);
delay(1500);

digitalWrite(5,HIGH);
delay(1500);

digitalWrite(2,LOW);
delay(1500);

digitalWrite(3,LOW);
delay(1500);

digitalWrite(4,LOW);
delay(1500);

digitalWrite(5,LOW);
delay(1500);

digitalWrite(2,HIGH);
digitalWrite(3,HIGH);
delay(1500);

digitalWrite(2,LOW);
digitalWrite(3,LOW);
delay(500);
digitalWrite(2,HIGH);
digitalWrite(4,HIGH);
delay(1500);

digitalWrite(2,LOW);
digitalWrite(4,LOW);
delay(500);
digitalWrite(2,HIGH);
digitalWrite(5,HIGH);
delay(1500);

digitalWrite(2,LOW);
digitalWrite(5,LOW);
delay(500);
digitalWrite(3,HIGH);
delay(500);
digitalWrite(4,HIGH);
delay(1500);

digitalWrite(3,LOW);
delay(250);
digitalWrite(4,LOW);
delay(500);
digitalWrite(2,HIGH);
delay(500);
digitalWrite(3,HIGH);
delay(500);
digitalWrite(4,HIGH);
delay(1500);

digitalWrite(2,LOW);
delay(500);
digitalWrite(3,LOW);
delay(500);
digitalWrite(4,LOW);
delay(500);
digitalWrite(2,HIGH);
digitalWrite(3,HIGH);
digitalWrite(5,HIGH);
delay(1500);

digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(5,LOW);
delay(500);
digitalWrite(3,HIGH);
digitalWrite(4,HIGH);
digitalWrite(5,HIGH);
delay(1500);

digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
delay(500);
digitalWrite(2,HIGH);
digitalWrite(3,HIGH);
digitalWrite(4,HIGH);
delay(1500);

digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
delay(500);
digitalWrite(2,HIGH);
digitalWrite(4,HIGH);
digitalWrite(5,HIGH);
delay(1500);

digitalWrite(2,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
delay(500);
digitalWrite(2,HIGH);
digitalWrite(3,HIGH);
digitalWrite(4,HIGH);
digitalWrite(5,HIGH);
delay(1500);

digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
delay(3000);


}
