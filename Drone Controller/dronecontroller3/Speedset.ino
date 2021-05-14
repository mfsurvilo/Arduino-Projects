#include <Servo.h>
#include "MyHeader.h"

void Speedset(int A,int B){
B = -1*A +3000;


  motor1.writeMicroseconds(throttle * A);
  motor2.writeMicroseconds(throttle*B);
}
