#include <Servo.h>
int pot_in = A7;
Servo motor1;
Servo motor2;

void setup() {
// put your setup code here, to run once:

Serial.begin(9600);

pinMode (pot_in,INPUT);

motor1.attach(11);
motor2.attach(12);

//Initialize setup
motor1.writeMicroseconds(1000);
motor2.writeMicroseconds(1000);
delay(2000);
}

void loop() {  
motor1.writeMicroseconds(1600);
motor2.writeMicroseconds(1200);
Serial.println("1600");
delay(4000);
motor1.writeMicroseconds(1200);
motor2.writeMicroseconds(1600):
Serial.println("1200");
delay(4000);


}
