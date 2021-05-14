void LCDDualPan(const String& mystring,const String& mystring2)
{
  lcd.clear();
  lcd.print(mystring); 
  lcd.setCursor(0,1);
  lcd.print(mystring2);
  int shift1 = mystring.length()-16;
  int shift2 = mystring2.length()-16;
  int delay1 = 0;                   // this delay changes every so often
  unsigned long cMillis = millis(); // current millis constantly updates
  unsigned long  sMillis = cMillis; // start millis is declared every once in a while
  buttonstate = 0;

  int shift = max(shift1, shift2);

  if (shift > 0) {
    for (int rep = 0; rep<2; rep++){
      for (int positionCounter = 0; positionCounter<shift; positionCounter++){
       lcd.scrollDisplayLeft();
       delay(300);
       }
     
       lcd.clear();
       lcd.print(mystring); 
       lcd.setCursor(0,1);
       lcd.print(mystring2);
       delay(1000);
    }
  } 
}
