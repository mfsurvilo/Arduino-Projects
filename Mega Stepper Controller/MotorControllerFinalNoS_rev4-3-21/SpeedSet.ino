// Sets speed of motor by changing pulse width. Input is RPM and pulses per revolution 

int SpeedSet(int RPM, int ppr) {

  
  int PDelay = 60*100000 / 2 / ppr / RPM;  

  /* Milliseconds in one second 
     Divided by two because there is a low and a high for each cycle.
     Divided by the pulse per rev and the desired RPM
  */
  
  if (PDelay < 200) {
    PDelay = 200;   // Sets minimum pulse delay if speed is set too high. 
  }
//Serial.print("Speed has been reset to. . . ");
//Serial.println(PDelay);
  return PDelay ;
}
