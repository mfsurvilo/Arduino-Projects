// Takes you to a home position as designated by homing switch.
  void Home() {  
    //Serial.println("In Home function") ;
    while (buttonState == 0) {
      SlowStep();
    }
}
