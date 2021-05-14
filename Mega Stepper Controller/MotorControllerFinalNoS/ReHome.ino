// Gets to position 1 quickly, stops at home button push, backs up and goes forward slowly till button is pushed. 

void Rehome(int Steps , String& Direction, int ppr) {
String OpDir;
   
  while (buttonState != 1) {
    digitalWrite(enPin,HIGH);
    delay(125);                    // This is a necessary signal delay (100ms minimum).
    if (Direction == CW) {         // Set Direction
      digitalWrite(dirPin,HIGH);
      OpDir = CCW;
    }
    else {
      digitalWrite(dirPin,LOW);
      OpDir = CW;
    } 
    delayMicroseconds(10);         // Delay between direction and pulses. (5us minimum)
    for (int x = 0; x < Steps; x++) {
      digitalWrite(pulPin,HIGH);
      delayMicroseconds(PDelay);   // This step effectively sets the speed of the motor. 
      digitalWrite(pulPin,LOW);    // Speed depends on the pause between pulses
      delayMicroseconds(PDelay);
    }
    valve = destvalve;             // Resets current position. 
    digitalWrite(enPin,LOW);       // Disables drive to minimize current draw. 
  }

  // Reverses 1/16th of a turn. 
  
  Steps = ppr/16;         
  SpeedSet(RPM/10,ppr);            // Slows down movement
  Step(Steps,OpDir,PDelay);

  // Moves forward slowly till buttonState var is changed. 
  
   while (buttonState != 1) {
    digitalWrite(enPin,HIGH);
    delay(125);                    // This is a necessary signal delay (100ms minimum).
    if (Direction == CW) {         // Set Direction
      digitalWrite(dirPin,HIGH);
    }
    else {
      digitalWrite(dirPin,LOW);
    } 
    delayMicroseconds(10);         // Delay between direction and pulses. (5us minimum)
    for (int x = 0; x < Steps; x++) {
      digitalWrite(pulPin,HIGH);
      delayMicroseconds(PDelay);   // This step effectively sets the speed of the motor. 
      digitalWrite(pulPin,LOW);    // Speed depends on the pause between pulses
      delayMicroseconds(PDelay);
    }
    valve = destvalve;             // Resets current position. 
    digitalWrite(enPin,LOW);       // Disables drive to minimize current draw. 
  }

   // Reset Speed
 SpeedSet(RPM, ppr);
}
