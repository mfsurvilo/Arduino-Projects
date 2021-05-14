// If there is a motor controller fault occurs everything halts and  the LEDPin blinks rapidly.
  void Fault_ISR() {
    while(1) {
      digitalWrite(LEDPin,HIGH);
      delay(250);
      digitalWrite(LEDPin,LOW);
      delay(250);
    }
}
