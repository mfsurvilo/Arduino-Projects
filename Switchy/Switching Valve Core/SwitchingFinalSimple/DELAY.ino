int DELAY(int howlong){    

        unsigned long cMillis = millis(); // start millis is declared every once in a while

        unsigned long sMillis = cMillis;
      while ((cMillis - sMillis) < howlong) {
         buttonstate = AOrB();
         cMillis = millis();
         } 
       return buttonstate;
}
