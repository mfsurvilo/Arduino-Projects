// Moves motor a certain number of Steps with a direction. Speed is set elsewhere. 

void Step(int Steps , String& Direction) { 

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
      digitalWrite(LEDPin, HIGH);
      delayMicroseconds(PDelay);   // This step effectively sets the speed of the motor. 
      digitalWrite(pulPin,LOW);    // Speed depends on the pause between pulses
      digitalWrite(LEDPin, LOW);
      delayMicroseconds(PDelay);
    }
    valve = destvalve;             // Resets current position. 
     
    digitalWrite(enPin,LOW);       // Disables drive to minimize current draw. 
}
