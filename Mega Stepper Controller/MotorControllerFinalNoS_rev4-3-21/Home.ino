// Takes you to a home position as designated by homing switch.
  void Home() { 
    //Serial.print(PDelay);
    //while (buttonState == 0) {
     //  Step(20,CW,PDelay);
       
    digitalWrite(dirPin,LOW);
    delayMicroseconds(10);         // Delay between direction and pulses. (5us minimum)
   
 while(buttonState ==0){
      digitalWrite(pulPin,LOW);    // Speed depends on the pause between pulses
      delayMicroseconds(PDelay);      // This step effectively sets the speed of the motor.
      digitalWrite(pulPin,HIGH);
      delayMicroseconds(PDelay);      // This step effectively sets the speed of the motor.
    }

}
