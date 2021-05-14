// Moves motor a certain number of Steps with a direction. Speed is set elsewhere. 

void Step(int Steps , String& Direction, float PDelay) { 
  const int pulPin = 5;     // Pulse signal.
    //Serial.print("Pdelay in step function is. . ");
    //Serial.println(PDelay);
   digitalWrite(enPin,LOW);
   delay(10);                    // This is a necessary signal delay (100ms minimum).
    if (Direction == CW) {         // Set Direction
      digitalWrite(dirPin,HIGH);
    }
    else {
      digitalWrite(dirPin,LOW);
    } 
    
    delayMicroseconds(10);         // Delay between direction and pulses. (5us minimum)
    
    for (int x = 0; x < Steps; x++) {
      float Factor;
 
      if(x<1000){
        Factor = (2000-x);        
      }
     else if (x> 4000) {
        Factor = 1000+x-4000;
      }
      else { Factor = 1000;
      }
 //Serial.print(x);
// Serial.print("  ");     
 //Serial.println(PDelay*Factor/1000);
      digitalWrite(pulPin,LOW);    // Speed depends on the pause between pulses
      delayMicroseconds(PDelay*Factor/1000);      // This step effectively sets the speed of the motor.
      digitalWrite(pulPin,HIGH);
      delayMicroseconds(PDelay*Factor/1000);      // This step effectively sets the speed of the motor.
    }
    
    valve = destvalve;             // Resets current position. 
     
    digitalWrite(enPin,HIGH);       // Disables drive to minimize current draw. 
}
