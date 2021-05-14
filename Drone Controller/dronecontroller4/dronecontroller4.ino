#include <Servo.h>
int pot_in = A1;
Servo motor1;
Servo motor2;

void setup() {
// put your setup code here, to run once:

Serial.begin(9600);

pinMode (pot_in,INPUT);

motor1.attach(5);
motor2.attach(4);

}

void loop() {  
motor1.writeMicroseconds(1200);

}
