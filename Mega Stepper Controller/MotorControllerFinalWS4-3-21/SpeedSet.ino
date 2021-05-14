// Sets speed of motor by changing pulse width. Input is RPM and pulses per revolution 

int SpeedSet(int RPM, int ppr) {

  
  int PDelay = 60*10^6 / 2 / ppr / RPM;  
  /* Milliseconds in one second
     Divided by two because there is a low and a high for each cycle.
     Divided by the pulse per rev and the desired RPM
  */

  if (PDelay < 100) {
    PDelay = 100;   // Sets minimum pulse delay if speed is set too high. 
  }

  return PDelay;
}
