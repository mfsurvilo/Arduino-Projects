unsigned long start1 = 0;

void setup() {
  // put your setup code here, to run once:
int pot_in = A1;
int pwmsig = 5;

  Serial.begin(9600);
  Serial.print(" ");

pinMode (pot_in,INPUT);

}

void loop() {

  unsigned long resistance = analogRead(A1);

 
  unsigned long highstate = map(resistance,0,560,1000,2000);

  delayMicroseconds(50);

  digitalWrite (5, HIGH);
  delayMicroseconds(highstate);
  digitalWrite (5,LOW);
  delayMicroseconds(20000-highstate);
  

}
